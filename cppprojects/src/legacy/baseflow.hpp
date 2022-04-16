#pragma once
#include "array.hpp"
#include "variable.hpp"

class BaseFlow {
public:
    Array2D<double> u_, v_, rho_, p_, T_;

    int Initialize();
    int CopyFlow(Array2D<variable> variables );
};
