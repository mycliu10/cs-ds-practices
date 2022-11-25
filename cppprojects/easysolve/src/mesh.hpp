#pragma once
#include "field.hpp"
#include "scheme.hpp"
#include "boundary_condition.hpp"

class Mesh {
protected:
    Index * index;
    BoundaryConditionsSet * boundaryConditionsSet;
    Field x;
    Field dxdi;

public:
    Mesh(Index * index, BoundaryConditionsSet * boundaryConditionsSet) {
        this->index = index;
        this->boundaryConditionsSet = boundaryConditionsSet;
        x.resize(index, boundaryConditionsSet);
        dxdi.resize(index, NULL);
    }

    Index * getIndex() {
        return index;
    }

    double const getX(vector<int> const & point) const {
        return x.getElement(point);
    }

    virtual double const getDxdi(vector<int> const & point) const {
        return dxdi.getElement(point);
    }
};

class UniformMesh : public Mesh {
    double lx;
    double dx;

public:
    UniformMesh(Index * index, BoundaryConditionsSet * boundaryConditionsSet, double lx) 
        : Mesh(index, boundaryConditionsSet) {
        this->lx = lx;
        this->dx = lx / index->getNumCell(0);
        x.resize(index, boundaryConditionsSet);
        for (auto gen = index->getAllZone(0); gen.isValid(); gen.next()) {
            int const i = gen.getCurrent();
            x.setElement({ i }, dx * (i - index->getBegin(0)));
        }
    }

    double const getDxdi(vector<int> const & point) const {
        return dx;
    }
};
