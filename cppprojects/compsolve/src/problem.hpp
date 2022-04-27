#pragma once
#include <iostream>
#include "mpi.h"
#include "array.hpp"
#include "domain.hpp"
#include "ideal_gas.hpp"
#include "mesh.hpp"
#include "flow.hpp"
#include "RHS.hpp"


class Problem {
public:
    IdealGas * gas_;
    Domain * domain_;
    Mesh * mesh_;
    Flow * flow_;
    RHS * rhs_;

    Problem(IdealGas *const gas, Domain *const domain, 
            Mesh *const mesh,    Flow *const flow,
            RHS *const rhs) {
        gas_ = gas;
        domain_ = domain;
        mesh_ = mesh;
        flow_ = flow;
        rhs_ = rhs;
    }
};
