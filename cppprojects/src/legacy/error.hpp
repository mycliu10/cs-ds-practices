#pragma once
#include "array.hpp"
#include "everything.hpp"

class ErrorEvaluator {
private:
    Array2D<double> rho_;
public:
    int InitializeRho();
    int RecordRho(FlowField & flow);
    double GetDrho(FlowField & flow);
};


