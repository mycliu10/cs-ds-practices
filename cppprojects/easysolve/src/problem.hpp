#pragma once
#include "mesh.hpp"
#include "advancement.hpp"
#include "residual.hpp"

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
    Residual residual;

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

    void solveI(IntGenerator & iGen) {
        vector<int> currentPoint(1);
        vector<int> workingPoint(1);

        // Spatial differential
        Timer::start("SpatialDifferential");
        for(iGen.reset(); iGen.isValid(); iGen.next()) {
            int i = iGen.getCurrent();
            currentPoint[0] = i;
            workingPoint[0] = i + interiorScheme->getOffset();
            auto it = temperature.getIterator(workingPoint);
            double ddf = interiorScheme->compute(it);
            ddf *= mesh->getD2idx2(currentPoint);
            rhs.element(currentPoint) = ddf;
        }
        Timer::stop("SpatialDifferential");
    }

    void advance(IntGenerator & iGen, double dt) {
        vector<int> currentPoint(1);

        for(iGen.reset(); iGen.isValid(); iGen.next()) {
            int i = iGen.getCurrent();
            currentPoint[0] = i;
            double & u = temperature.element(currentPoint);
            double & du = rhs.element(currentPoint);
            u = advancement->advance(u, du, dt);
            residual.compute(du);
        }
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
            residual.reset();

            // Boundary conditions
            Timer::start("BoundaryConditions");
            int numSkipPointsBegin = solveBoundaryCondition(BEGIN, 0);
            int numSkipPointsEnd = solveBoundaryCondition(END, 0);
            iGen.setSkipBegin(numSkipPointsBegin);
            iGen.setSkipEnd(numSkipPointsEnd);
            Timer::stop("BoundaryConditions");

            // Solve I line
            solveI(iGen);

            // Time advancement
            Timer::start("TimeAdvancement");
            advance(iGen, dt);
            Timer::stop("TimeAdvancement");

            if (countIterations % 1000 == 0) {
                cout << "At step = " << std::setw(9) << countIterations;
                cout << residual.report().str();
                cout << endl;
            }
            if (residual.getLmaxResidual() < 1.e-6) {
                break;
            }
        }
    }
};

