#include "error.hpp"
#include "domain.hpp"
#include <cmath>

int ErrorEvaluator::InitializeRho() {
    rho_.Initialize(domain::ilen, domain::jlen);
}


int ErrorEvaluator::RecordRho(FlowField & flow) {
    for(int i=domain::ibeg; i<domain::iend; i++) {
        for(int j=domain::jbeg; j<domain::jend; j++) {
            rho_.Get(i,j) = flow.vars.Get(i,j).rho_;
        }
    }
    return 1;
}
        

double ErrorEvaluator::GetDrho(FlowField & flow) {
    double drho_max = 0.;
    for(int i=domain::ibeg; i<domain::iend; i++) {
        for(int j=domain::jbeg; j<domain::jend; j++) {
            double drho_work = std::abs(flow.vars.Get(i,j).rho_ - rho_.Get(i,j));
            if(drho_work>drho_max) {
                drho_max = drho_work;
            }
        }
    }
    return drho_max;
}
        


