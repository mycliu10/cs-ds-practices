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

public:
    BoundaryCondition * getBeginBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ 0, dimension });
    }

    BoundaryCondition * getEndBoundaryCondition(int dimension) {
        return boundaryConditions.getElement({ 1, dimension });
    }
};


