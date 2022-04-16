#include "domain.hpp"
#include "init.hpp"
#include "everything.hpp"
#include "compute.hpp"

#include <iostream>

using namespace std;

int init::allocate(FlowField & flow){
    flow.vars.Initialize(domain::ilen, domain::jlen);
    flow.convs.Initialize(domain::ilen, domain::jlen);
    flow.viscs.Initialize(domain::ilen, domain::jlen);
    flow.rhss.Initialize(domain::ilen, domain::jlen);
    flow.meshs.Initialize(domain::ilen, domain::jlen);
    cout << "Initialized flow." << endl;
    return 0;
}


int init::work_array(){
    NativeArray2D(compute::fflux_iextra, domain::inpt+1, rhs::num_var);
    compute::pflux_iextra = new double[domain::inpt+1];
    NativeArray2D(compute::f_iextra, domain::inpt+2, rhs::num_var);
    compute::p_iextra = new double*[domain::inpt+2];
    compute::u_iextra = new double*[domain::inpt+2];
    compute::a_iextra = new double*[domain::inpt+2];
    compute::M_iextra = new double[domain::inpt+2]; 
    NativeArray2D(compute::fflux_jextra, domain::jnpt+1, rhs::num_var);
    compute::pflux_jextra = new double[domain::jnpt+1];
    NativeArray2D(compute::f_jextra, domain::jnpt+2, rhs::num_var);
    compute::p_jextra = new double*[domain::jnpt+2];
    compute::u_jextra = new double*[domain::jnpt+2];
    compute::a_jextra = new double*[domain::jnpt+2];
    compute::M_jextra = new double[domain::jnpt+2]; 
    return 1;
}
