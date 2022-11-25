#pragma once
#include "mesh.hpp"
#include "advancement.hpp"

class Problem {
protected:
    Mesh * mesh;
    Advancement * advancement;
public:
    void initialize(Mesh * mesh, Advancement * advancement) {
        this->mesh = mesh;
        this->advancement = advancement;
    }

    virtual void solve(double dt) = 0;

    virtual ~Problem() {}
};


class PoissonProblem : public Problem {
    SecondOrderDerivativeScheme * interiorScheme;
    Field temperature;
    Field rhs;

public:
    void initialize(Mesh * mesh, Advancement * advancement,
            BoundaryConditionsSet * temperatureBoundaryConditions) {
        Problem::initialize(mesh, advancement);
        interiorScheme = new CentralDifferenceSecondOrder();
        temperature.resize(mesh->getIndex(), temperatureBoundaryConditions);
        rhs.resize(mesh->getIndex(), NULL);
    }

    void solve(double dt) {
        Index * index = mesh->getIndex();
        IntGenerator iGen = index->getInteriorZone(0);
        IntGenerator schemeGen = interiorScheme->getStencilGenerator();
        vector<double> temperatureStencil(interiorScheme->getStencilSize());
        vector<int> currentPoint(1);
        vector<int> workingPoint(1);

        for(int countIterations = 0; countIterations < 2; ++countIterations) {
            cout << temperature.element({0}) << " " << temperature.element({1}) << endl;
            for(iGen.reset(); iGen.isValid(); iGen.next()) {
                int i = iGen.getCurrent();
                currentPoint[0] = i;
                if (i == index->getBegin(0)) {
                    double & f = temperature.element(currentPoint);
                    double df;
                    if (temperature.getBeginBoundaryCondition(0)->compute(f, df) == DERIVATIVE_CHANGE) {
                        vector<int> point1 = { i - 1 };
                        vector<int> point2 = { i + 1 };
                        temperature.element(point1) = - df * mesh->getDxdi(currentPoint) + temperature.element(point2);
                    }
                } else if(i == index->getEnd(0)) {
                    double & f = temperature.element(currentPoint);
                    double df;
                    if (temperature.getBeginBoundaryCondition(0)->compute(f, df) == DERIVATIVE_CHANGE) {
                        vector<int> point1 = { i - 1 };
                        vector<int> point2 = { i + 1 };
                        temperature.element(point2) = df * mesh->getDxdi(currentPoint) + temperature.element(point1);
                    }
                }
                for(schemeGen.reset(); schemeGen.isValid(); schemeGen.next()) {
                    int n = i + schemeGen.getCurrent();
                    int count = schemeGen.getCount();
                    workingPoint[0] = n;
                    temperatureStencil[count] = temperature.element(workingPoint);
                }
                double ddf = interiorScheme->compute(temperatureStencil);
                ddf *= - mesh->getDxdi(currentPoint);
                rhs.element(currentPoint) = ddf;
            }
            for(iGen.reset(); iGen.isValid(); iGen.next()) {
                int i = iGen.getCurrent();
                currentPoint[0] = i;
                if (i == index->getBegin(0)) {
                    continue;
                } else if(i == index->getEnd(0)) {
                    continue;
                } else {
                    double & u = temperature.element(currentPoint);
                    double & du = rhs.element(currentPoint);
                    u = advancement->advance(u, du, dt);
                }
            }
        }
    }
};

