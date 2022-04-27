#pragma once
#include <iostream>
#include "problem.hpp"
#include "conv.hpp"
#include "visc.hpp"
#include "BC.hpp"

class DNSRun {
private:
    Problem * p_;
    BCRun * bcrun_;
    ConvectionRun * crun_;
    ViscousRun * vrun_;
public:
    DNSRun(Problem * p, BCRun *const bcrun, ConvectionRun *const crun, ViscousRun *const vrun);
    int EvaluateRHS();
    int Integrate(const double dt, const double physical_dt);
    int ScaleRHS(const double f);
};

class DNS {
public:
    static int ComputeRHS(const Domain &domain, const Mesh &mesh, Flow &flow, RHS &rhs, const IdealGas &gas);

};
