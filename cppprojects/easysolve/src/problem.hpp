#pragma once
#include "mesh.hpp"
#include "advancement.hpp"

class Problem {
    Mesh * mesh;
    Advancement * advancement;
public:
    void initialize(Mesh * mesh, Advancement * advancement) {
        this->mesh = mesh;
        this->advancement = advancement;
    }

    virtual void solve() = 0;
};


class PoissonProblem : public Problem {
    SecondOrderDerivativeScheme * interiorScheme;
    Field temperature;

public:
    void initialize(Mesh * mesh, Advancement * advancement,
            BoundaryConditionsSet * temperatureBoundaryConditions) {
        Problem::initialize(mesh, advancement);
        interiorScheme = new CentralDifferenceSecondOrder();
        temperature.resize(mesh->getIndex(), temperatureBoundaryConditions);
    }

    void solve() {
        Index * index = mesh->getIndex();
        IntGenerator index->getInteriorZone(0);
        for(
        

    }
};

