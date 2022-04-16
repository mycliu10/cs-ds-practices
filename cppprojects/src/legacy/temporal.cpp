#include "temporal.hpp"
#include "domain.hpp"
#include "everything.hpp"
#include <cmath>
#include <limits>

double temporal::dt;

int temporal::Euler(FlowField & flow, int (*step)(FlowField & flow, BoundaryCondition & BC), BoundaryCondition & BC, double & dt ){
    int err = 1;
    err = flow.reset_rhss(); 

    err = step(flow, BC);
    err = flow.update_vars(dt);
    flow.time_ += dt;
    return err; 

}


int temporal::check_CFL(FlowField & flow, double & CFL) {
    int err;
    variable * var;
    MeshPoint * msh;
    double CFL_max = 0.;

    for(int i=0; i<domain::ilen; i++) {
        for(int j=0; j<domain::jlen; j++) {
            var = &flow.vars.Get(i,j);
            msh = &flow.meshs.Get(i,j);
            double u0, u1, a0, a1;
            u0 = (*msh).DifferentiateFirstOrder((*var).u_, 0);
            a0 = (*msh).DifferentiateFirstOrder((*var).a_, 0);
            u1 = (*msh).DifferentiateFirstOrder((*var).v_, 1);
            a1 = (*msh).DifferentiateFirstOrder((*var).a_, 1);
            CFL = std::abs(u0) + a0 + std::abs(u1) + a1;
            if(CFL>CFL_max) {
                CFL_max = CFL;
            }
        }
    }

    CFL = 1. / CFL_max;
    return err;
}


int temporal::constant_CFL(FlowField & flow, const double CFL, double & dt) {
    double CFL_work;
    int err = check_CFL(flow, CFL_work);
    dt = CFL*CFL_work;
    return err;
}




