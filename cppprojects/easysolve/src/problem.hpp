#pragma once
#include "mesh.hpp"
#include "advancement.hpp"

class Problem {
protected:
    shared_ptr<Index> index;
    shared_ptr<Mesh> mesh;
    shared_ptr<Advancement> advancement;
public:
    void initialize(shared_ptr<Mesh> mesh, shared_ptr<Advancement> advancement) {
        this->mesh = mesh;
        index = mesh->getIndex();
        this->advancement = advancement;
    }

    virtual void solve(double dt, int numIterations) = 0;

    virtual ~Problem() {}
};


class PoissonProblem : public Problem {
    unique_ptr<SecondOrderDerivativeScheme> interiorScheme;
    Field temperature;
    Field rhs;
    double l1Error;
    double l2Error;
    double lmaxError;

    int solveBoundaryCondition(BoundaryConditionSide side, int dimension) {
        int i;
        if (side == BEGIN) {
            i = index->getBegin(dimension);
        } else {
            i =  index->getEnd(dimension);
        }

        vector<int> point = { i };
        BoundaryConditionComputeChange change = temperature.getBoundaryCondition(side, dimension)->getChange();
        if (change == VALUE_CHANGE) {
            double & f = temperature.element(point);
            double df;
            temperature.getBoundaryCondition(side, dimension)->compute(f, df);
            return 1;
        } else if (change == DERIVATIVE_CHANGE) {
            double f, df;
            temperature.getBoundaryCondition(side, dimension)->compute(f, df);
            if (side == BEGIN) {
                temperature.element({ i - 1 }) = temperature.element({ i + 1 }) - 2. * df / mesh->getDidx(point);
            } else {
                temperature.element({ i + 1 }) = temperature.element({ i - 1 }) + 2. * df / mesh->getDidx(point);
            }
            return 0;
        }
        return 0;
    }

public:
    void initialize(shared_ptr<Mesh> mesh, shared_ptr<Advancement> advancement,
            shared_ptr<BoundaryConditionsSet> temperatureBoundaryConditions) {
        Problem::initialize(mesh, advancement);
        interiorScheme.reset(new CentralDifferenceSecondOrder());
        temperature.resize(mesh->getIndex(), temperatureBoundaryConditions);
        rhs.resize(mesh->getIndex(), NULL);
    }

    void solve(double dt, int numIterations) {
        IntGenerator iGen = index->getInteriorZone(0);
        IntGenerator schemeGen = interiorScheme->getStencilGenerator();
        vector<double> temperatureStencil(interiorScheme->getStencilSize());
        vector<int> currentPoint(1);
        vector<int> workingPoint(1);

        for(int countIterations = 1; countIterations <= numIterations; ++countIterations) {
            l1Error = 0.;
            l2Error = 0.;
            lmaxError = 0.;
//            for (int i=0; i<8; ++i) {
//                cout << temperature.element({i}) << " ";
//            }
//            cout << endl;

            // Boundary conditions
            Timer::start("BoundaryConditions");
            int numSkipPointsBegin = solveBoundaryCondition(BEGIN, 0);
            int numSkipPointsEnd = solveBoundaryCondition(END, 0);
            Timer::stop("BoundaryConditions");

            // Spatial differential
            Timer::start("SpatialDifferential");
            for(iGen.reset(); iGen.isValid(); iGen.next()) {
                int i = iGen.getCurrent();
                if (i < index->getBegin(0) + numSkipPointsBegin || i > index->getEnd(0) - numSkipPointsEnd) {
                    continue;
                }
                currentPoint[0] = i;
                Timer::start("StencilVector");
                for(schemeGen.reset(); schemeGen.isValid(); schemeGen.next()) {
                    int n = i + schemeGen.getCurrent();
                    int count = schemeGen.getCount();
                    workingPoint[0] = n;
                    temperatureStencil[count] = temperature.element(workingPoint);
                }
                Timer::stop("StencilVector");
                double ddf = interiorScheme->compute(temperatureStencil);
                ddf *= mesh->getD2idx2(currentPoint);
                rhs.element(currentPoint) = ddf;
            }
            Timer::stop("SpatialDifferential");

            // Time advancement
            Timer::start("TimeAdvancement");
            for(iGen.reset(); iGen.isValid(); iGen.next()) {
                int i = iGen.getCurrent();
                if (i < index->getBegin(0) + numSkipPointsBegin || i > index->getEnd(0) - numSkipPointsEnd) {
                    continue;
                }
                currentPoint[0] = i;
                double & u = temperature.element(currentPoint);
                double & du = rhs.element(currentPoint);
                u = advancement->advance(u, du, dt);

                l1Error += std::abs(du);
                l2Error += du*du;
                lmaxError = std::max(lmaxError, std::abs(du));
            }
            Timer::stop("TimeAdvancement");

            if (countIterations % 1000 == 0) {
                cout << "At step = " << countIterations << " L1/L2/Lmax error = " 
                        << l1Error << "/" << l2Error << "/" << lmaxError << endl;
            }
            if (lmaxError < 1.e-9) {
                break;
            }
        }
    }
};
