#pragma once
#include "head.hpp"


enum BoundaryConditionSide {
    BEGIN,
    END
};

enum BoundaryConditionComputeChange {
    NO_CHANGE,
    VALUE_CHANGE,
    DERIVATIVE_CHANGE
};


class BoundaryCondition {
public:
    virtual void compute(double & f, double & df) = 0;

    virtual BoundaryConditionComputeChange getChange() {
        return NO_CHANGE;
    }

    virtual ~BoundaryCondition() {}
};


class DummyBoundaryCondition : public BoundaryCondition {
    void compute(double & f, double & df) {
        (void) f;
        (void) df;
    }
};


class DirichletBoundaryCondition : public BoundaryCondition {
    double fValue;

public:
    DirichletBoundaryCondition(double fValue) : fValue(fValue) {}

    BoundaryConditionComputeChange getChange() {
        return VALUE_CHANGE;
    }

    void compute(double & f, double & df) {
        f = fValue;
        (void) df;
    }
};


class NeumannBoundaryCondition : public BoundaryCondition {
    double dfValue;

public:
    NeumannBoundaryCondition(double dfValue) : dfValue(dfValue) {}

    BoundaryConditionComputeChange getChange() {
        return DERIVATIVE_CHANGE;
    }

    void compute(double & f, double & df) {
        (void) f;
        df = dfValue;
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
    shared_ptr<BoundaryCondition> getBoundaryCondition(BoundaryConditionSide side, int dimension) {
        return boundaryConditions.getElement({ dimension, side });
    }
};


