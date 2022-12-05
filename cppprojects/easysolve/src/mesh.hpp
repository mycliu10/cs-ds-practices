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

    double getX(vector<int> & point) {
        return x.element(point);
    }

    virtual double getDidx(vector<int> & point) {
        return didx.element(point);
    }

    virtual double getD2idx2(vector<int> & point) const = 0;

    virtual ~Mesh() {}
};

class UniformMesh : public Mesh {
    double lx;
    double dx;
    double didx;
    double d2idx2;

public:
    UniformMesh(shared_ptr<Index> index, shared_ptr<BoundaryConditionsSet> boundaryConditionsSet, double lx) 
        : Mesh(index, boundaryConditionsSet) {
        this->lx = lx;
        dx = lx / index->getNumCell(0);
        x.resize(index, boundaryConditionsSet);
        for (auto gen = index->getAllZone(0); gen.isValid(); gen.next()) {
            int const i = gen.getCurrent();
            x.element({ i }) = dx * (i - index->getBegin(0));
        }
        didx = 1. / dx;
        d2idx2 = didx * didx;
    }

    double getDidx(vector<int> & point) {
        (void) point;
        return didx;
    }

    double getD2idx2(vector<int> & point) const {
        (void) point;
        return d2idx2;
    }
};
