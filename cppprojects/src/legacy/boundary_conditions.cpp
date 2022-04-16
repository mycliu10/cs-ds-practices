#include "BC.hpp"
#include "eigeninflow.hpp"

int BoundaryConditionJ::Periodic(FlowField & flow) {
    if(domain::jbeg_global==0 && domain::jend_global==domain::jmax){
        for(int i=domain::ibeg; i<domain::iend; i++){
            for(int n=0; n<domain::stencilsize; n++){
                flow.vars.Get(i,n).CopyAll(flow.vars.Get(i,n+domain::jnpt));
                flow.vars.Get(i,n+domain::jend).CopyAll(flow.vars.Get(i,n+domain::jbeg));
            }
        }
    }
}


int BoundaryConditionI::ExtrapolateZerothOrderEnd(FlowField & flow) {
    if(domain::iend_global==domain::imax) {
        for(int j=domain::jbeg; j<domain::jend; j++){ 
            flow.vars.Get(domain::iend-1,j).CopyAll(flow.vars.Get(domain::iend-2,j));
        }
    }
}


int BoundaryConditionI::ForceEigenInflow(FlowField & flow) {
    if(domain::ibeg_global==0) {
        double time = flow.time_;
        eigen_inflow_.ForceEigenInflow(flow, time); 
    }

}
