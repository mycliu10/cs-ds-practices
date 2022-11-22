#pragma once
#include "field.hpp"
#include "scheme.hpp"
#include "boundary_condition.hpp"

class Mesh {
protected:
    Index * index;
    VectorN<BoundaryCondition *> boundaryConditions;
    Field x;
    Field dxdi;

public:
    class Builder {
    public:
        Index * index;
        VectorN<BoundaryCondition *> boundaryConditions;

        Builder(Index * index) : index(index), 
                boundaryConditions({ 2, index->getDimension() }) {}

        Builder & setBeginBoundaryCondition(BoundaryCondition * bc, int const dimension) {
            boundaryConditions.setElement({ 0, dimension }, bc);
            return *this;
        }

        Builder & setEndBoundaryCondition(BoundaryCondition * bc, int const dimension) {
            boundaryConditions.setElement({ 1, dimension }, bc);
            return *this;
        }

        virtual Mesh build() {
            return Mesh(*this);
        }
    };

protected:
    Mesh(Builder builder) {
        index = builder.index;
        boundaryConditions = builder.boundaryConditions;
    }

public:
    virtual void initialize() {
        x.resize(index);
        dxdi.resize(index);
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
    class Builder : public Mesh:: Builder {
    public:
        double lx;
        Builder & setLx(double const lx) {
            this->lx = lx;
            return *this;
        }

        Mesh build() {
            return UniformMesh(*this);
        }
    };

protected:
    UniformMesh(Builder & builder) : Mesh(builder) {
        lx = builder.lx;
        dx = lx / index->getNumCell(0);
    }

public:
    void initialize() {
        x.resize(index);
        for (auto gen = index->getAllZone(0); gen.isValid(); gen.next()) {
            int const i = gen.getCurrent();
            x.setElement({ i }, dx * (i - index->getBegin(0)));
        }
    }

    double const getDxdi(vector<int> const & point) const {
        return dx;
    }
};
