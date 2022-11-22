#pragma once
#include "head.hpp"

class BoundaryCondition {
public:
    virtual void compute(double & f, double & df) = 0;

    virtual ~BoundaryCondition() {}
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
