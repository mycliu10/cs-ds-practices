#pragma once
#include "head.hpp"


enum BoundaryConditionComputeChange {
    NO_CHANGE,
    VALUE_CHANGE,
    DERIVATIVE_CHANGE
};


class BoundaryCondition {
public:
    virtual BoundaryConditionComputeChange compute(double & f, double & df) = 0;

    virtual int suggestHalo() {
        return 0;
    }

    virtual ~BoundaryCondition() {}
};


class DummyBoundaryCondition : public BoundaryCondition {
    BoundaryConditionComputeChange compute(double & f, double & df) {
        (void) f;
        (void) df;
        return NO_CHANGE;
    }
};


class DirichletBoundaryCondition : public BoundaryCondition {
    double fValue;

public:
    DirichletBoundaryCondition(double fValue) : fValue(fValue) {}

    BoundaryConditionComputeChange compute(double & f, double & df) {
        f = fValue;
        (void) df;
        return VALUE_CHANGE;
    }
};


class NeumannBoundaryCondition : public BoundaryCondition {
    double dfValue;

public:
    NeumannBoundaryCondition(double dfValue) : dfValue(dfValue) {}

    BoundaryConditionComputeChange compute(double & f, double & df) {
        (void) f;
        df = dfValue;
        return DERIVATIVE_CHANGE;
    }
};


class BoundaryConditionsSet {
protected:
    VectorN<shared_ptr<BoundaryCondition>> boundaryConditions;

public:
    class Builder {
    public:
        int dimension;
        VectorN<shared_ptr<BoundaryCondition>> boundaryConditions;

        Builder(int dimension) : dimension(dimension) {
            boundaryConditions.resize({ dimension, 2 });
        }

        Builder & setBeginBoundaryCondition(shared_ptr<BoundaryCondition> bc, int const dimension) {
            boundaryConditions.setElement({ dimension, 0 }, bc);
            return *this;
        }

        Builder & setEndBoundaryCondition(shared_ptr<BoundaryCondition> bc, int const dimension) {
            boundaryConditions.setElement({ dimension, 1 }, bc);
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

public:
    shared_ptr<BoundaryCondition> getBeginBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ dimension, 0 });
    }

    shared_ptr<BoundaryCondition> getEndBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ dimension, 1 });
    }
};


