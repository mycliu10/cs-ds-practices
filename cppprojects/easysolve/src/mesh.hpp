#pragma once
#include "field.hpp"
#include "scheme.hpp"
#include "boundary_condition.hpp"

class Mesh {
protected:
    shared_ptr<Index> index;
    shared_ptr<BoundaryConditionsSet> boundaryConditionsSet;
    Field x;
    Field didx;

public:
    Mesh(shared_ptr<Index> index, shared_ptr<BoundaryConditionsSet> boundaryConditionsSet) {
        this->index = index;
        this->boundaryConditionsSet = boundaryConditionsSet;
        x.resize(index, boundaryConditionsSet);
        didx.resize(index, NULL);
    }

    shared_ptr<Index> getIndex() {
        return index;
    }

    double getX(vector<int> & point) const {
        return x.getElement(point);
    }

    virtual double getDidx(vector<int> & point) const {
        return didx.getElement(point);
    }

    virtual double getD2idx2(vector<int> & point) const = 0;

    virtual ~Mesh() {}
};

class UniformMesh : public Mesh {
    double lx;
    double dx;
    double dx2;

public:
    UniformMesh(shared_ptr<Index> index, shared_ptr<BoundaryConditionsSet> boundaryConditionsSet, double lx) 
        : Mesh(index, boundaryConditionsSet) {
        this->lx = lx;
        dx = lx / index->getNumCell(0);
        dx2 = 1. / (dx * dx);
        x.resize(index, boundaryConditionsSet);
        for (auto gen = index->getAllZone(0); gen.isValid(); gen.next()) {
            int const i = gen.getCurrent();
            x.element({ i }) = dx * (i - index->getBegin(0));
        }
        dx = 1. / dx;
    }

    double getDidx(vector<int> & point) const {
        (void) point;
        return dx;
    }

    double getD2idx2(vector<int> & point) const {
        (void) point;
        return dx2;
    }
};
