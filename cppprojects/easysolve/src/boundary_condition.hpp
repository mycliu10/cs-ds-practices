#pragma once
#include "head.hpp"



class BoundaryCondition {
public:
    virtual void compute(double & f, double & df) = 0;

    virtual int suggestHalo() {
        return 0;
    }

    virtual ~BoundaryCondition() {}
};


class DummyBoundaryCondition : public BoundaryCondition {
    void compute(double & f, double & df) {
        return;
    }
};


class DirichletBoundaryCondition : public BoundaryCondition {
    double fValue;

public:
    DirichletBoundaryCondition(double fValue) : fValue(fValue) {}

    void compute(double & f, double & df) {
        f = fValue;
    }
};


class NeumannBoundaryCondition : public BoundaryCondition {
    double dfValue;

public:
    NeumannBoundaryCondition(double dfValue) : dfValue(dfValue) {}

    void compute(double & f, double & df) {
        df = dfValue;
    }
};


class BoundaryConditionsSet {
protected:
    VectorN<BoundaryCondition *> boundaryConditions;

public:
    class Builder {
    public:
        int dimension;
        VectorN<BoundaryCondition *> boundaryConditions;

        Builder(int dimension) : dimension(dimension) {
            boundaryConditions.resize({ 2, dimension });
        }

        Builder & setBeginBoundaryCondition(BoundaryCondition * bc, int const dimension) {
            boundaryConditions.setElement({ 0, dimension }, bc);
            return *this;
        }

        Builder & setEndBoundaryCondition(BoundaryCondition * bc, int const dimension) {
            boundaryConditions.setElement({ 1, dimension }, bc);
            return *this;
        }

        BoundaryConditionsSet * build() {
            return new BoundaryConditionsSet(*this);
        }
    };

protected:
    BoundaryConditionsSet(Builder & builder) {
        boundaryConditions = builder.boundaryConditions;
    }

    BoundaryCondition * getBeginBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ 0, dimension });
    }

    BoundaryCondition * getEndBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ 1, dimension });
    }
};


