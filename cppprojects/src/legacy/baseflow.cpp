#include "baseflow.hpp"
#include "domain.hpp"
#include "variable.hpp"


int BaseFlow::Initialize() {
    int err;
    err = u_.Initialize(domain::ilen, domain::jlen);
    err = v_.Initialize(domain::ilen, domain::jlen);
    err = rho_.Initialize(domain::ilen, domain::jlen);
    err = p_.Initialize(domain::ilen, domain::jlen);
    err = T_.Initialize(domain::ilen, domain::jlen);
    return err;
}


int BaseFlow::CopyFlow(Array2D<variable> variables) {
    variable * pointer_var;
    for(int i=0; i<domain::ilen; i++){
        for(int j=0; j<domain::jlen; j++){
            pointer_var = &variables.Get(i,j);
            u_.Get(i,j) = (*pointer_var).u_;
            v_.Get(i,j) = (*pointer_var).v_;
            rho_.Get(i,j) = (*pointer_var).rho_;
            p_.Get(i,j) = (*pointer_var).p_;
            T_.Get(i,j) = (*pointer_var).T_;
        }
    }
    return 1;
}
