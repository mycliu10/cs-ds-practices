#pragma once
#include <iostream>
#include "flow.hpp"
#include "DNS.hpp"
#include "post.hpp"

class TimeIntegrator {
protected:
    DNSRun * run_;
    Post * post_;
    double dt_;
    double CFL_;
    int start_step_;
public:
    TimeIntegrator() {
        run_ = NULL;
        post_ = NULL;
    }
    virtual ~TimeIntegrator() {
        run_ = NULL;
        post_ = NULL;
    }
    int Setup(DNSRun *const run, Post *const post, const double dt, const double CFL);
    virtual int Kernel() = 0;
    int SetTimeStep(const double dt, const int count);
    int Run(const int start_step, const int num_steps);
};

class Euler : public TimeIntegrator {
private:
public:
    Euler() {};
    int Kernel();
};

class LowStorageRK3 : public TimeIntegrator {
private:
public:
    LowStorageRK3() {};
    int Kernel();
};




class Temporal {
public:
    static int Kernel(const Domain &domain, Flow &flow, const RHS &rhs, const IdealGas &gas, const double dt);
    static int Euler(const Domain &domain, const Mesh &mesh, Flow &flow, RHS &rhs, const IdealGas &gas, const double dt);
    static int LowStorageRK3(const Domain &domain, const Mesh &mesh, Flow &flow, RHS &rhs, const IdealGas &gas, const double dt);
};
