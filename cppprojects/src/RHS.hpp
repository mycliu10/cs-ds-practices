#pragma once
#include "array.hpp"
#include "domain.hpp"

class RHS {
private:
    Domain * domain_;
public:
    Array<double> dQ_, dQ0_;

    RHS(Domain *const domain); 

    int Zero(); 
    int Multiply(const double f);

    double GetNorm();
    
    
};
