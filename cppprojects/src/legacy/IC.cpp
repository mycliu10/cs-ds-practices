#include "IC.hpp"
#include "domain.hpp"
#include "variable.hpp"


int IC::init(FlowField & flow) {
    for(int i=0; i<domain::ibeg; i++) {
        for(int j=0; j<domain::jlen; j++) {
            flow.vars.Get(i,j).u_ = 223.;
            flow.vars.Get(i,j).v_ = 0.;
            flow.vars.Get(i,j).p_ = 670.;
            flow.vars.Get(i,j).T_ = 55.;
            flow.vars.Get(i,j).Get_everything_from_primitive();
        }
    }
    // interior
    for(int i=domain::ibeg; i<domain::ilen; i++) {
        for(int j=0; j<domain::jlen; j++) {
            flow.vars.Get(i,j).u_ = 223.;
            flow.vars.Get(i,j).v_ = 0.;
            flow.vars.Get(i,j).p_ = 670.;
            flow.vars.Get(i,j).T_ = 55.;
            flow.vars.Get(i,j).Get_everything_from_primitive();
        }
    }
}    
