#include "DNS.hpp"

DNSRun::DNSRun(Problem *const p, BCRun *const bcrun, 
               ConvectionRun *const crun, ViscousRun *const vrun) {
    p_ = p;
    bcrun_ = bcrun;
    crun_ = crun;
    vrun_ = vrun;
    std::cout << "DNS initialized." << std::endl;
    std::cout << "========================================================" << std::endl;
}

int DNSRun::EvaluateRHS() {
    bcrun_->RunY();
    crun_ ->RunY();

    bcrun_->RunZ();
    crun_ ->RunZ();

    bcrun_->RunX();
    crun_ ->RunX();

    vrun_ ->RunY();
    vrun_ ->RunZ();
    vrun_ ->RunX();
    vrun_ ->RunV();

    return 0;
}

int DNSRun::Integrate(const double dt, const double physical_dt) {
    global_profiler.Start(15);

    int ibeg = p_->domain_->ibeg_;
    int jbeg = p_->domain_->jbeg_;
    int kbeg = p_->domain_->kbeg_;
    int iend = p_->domain_->iend_;
    int jend = p_->domain_->jend_;
    int kend = p_->domain_->kend_;
    if(p_->domain_->ibegbc_==1) {
	    ibeg = p_->domain_->ibeg_+1;
    }
    if(p_->domain_->jbegbc_==1) {
	    jbeg = p_->domain_->jbeg_+1;
    }
    if(p_->domain_->kbegbc_==1) {
	    kbeg = p_->domain_->kbeg_+1;
    }

#pragma omp parallel for collapse(3)
    for(int i=ibeg; i<=iend; ++i) {
        for(int k=kbeg; k<=kend; ++k) {
            for(int j=jbeg; j<=jend; ++j) {
                int idx  = p_->flow_->rho_.GetIndex(i,k,j);
                int idx4 = p_->rhs_ -> dQ_.GetIndex(i,k,j,0);
                p_->flow_->  rho_.Get(idx) += p_->rhs_->dQ_.GetValue(idx4  )*dt;
                p_->flow_-> rhou_.Get(idx) += p_->rhs_->dQ_.GetValue(idx4+1)*dt;
                p_->flow_-> rhov_.Get(idx) += p_->rhs_->dQ_.GetValue(idx4+2)*dt;
                p_->flow_-> rhow_.Get(idx) += p_->rhs_->dQ_.GetValue(idx4+3)*dt;
                p_->flow_->rhoet_.Get(idx) += p_->rhs_->dQ_.GetValue(idx4+4)*dt;
   	            p_->flow_->UpdateFlowFromConservative(idx);
            }
        }
    }

    p_->flow_->time_ += physical_dt;

    global_profiler.Pause(15);
    return 0;
}

int DNSRun::ScaleRHS(const double f) {
    p_->rhs_->Multiply(f);
    return 0;
}


