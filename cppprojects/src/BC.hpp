#pragma once
#include "problem.hpp"

class BCRun {
private:
    Problem * p_;
public:
    BCRun(Problem *const p);
    int RunX();
    int RunY();
    int RunZ();
    int PeriodicX();
    int PeriodicY();
    int PeriodicZ();
    int ExtrapolateZerothXEnd();
};

class BC {
public:
    static int PeriodicX(const Domain &domain, Flow &flow, const IdealGas &gas);
    static int PeriodicY(const Domain &domain, Flow &flow, const IdealGas &gas);
    static int PeriodicZ(const Domain &domain, Flow &flow, const IdealGas &gas);
    static int ExtrapolateZerothXEnd(const Domain &domain, Flow &flow, const IdealGas &gas);

};


