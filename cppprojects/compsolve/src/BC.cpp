#include "BC.hpp"


BCRun::BCRun(Problem *const p) {
    p_ = p;
    std::cout << "BC initialized." << std::endl;
    std::cout << "========================================================" << std::endl;
 
}


int BCRun::RunX() {
    if(p_->domain_->ilen_global_==1) {
        return 1;
    }

    global_profiler.Start(4);

    switch(p_->domain_->iendbc_) {
        case(0):
            PeriodicX();
            break;
        case(2):
            ExtrapolateZerothXEnd();
            break;
    }

    global_profiler.Pause(4);
    return 0;
}

int BCRun::RunY() {
    if(p_->domain_->jlen_global_==1) {
        return 1;
    }

    global_profiler.Start(5);
    PeriodicY();
    global_profiler.Pause(5);

    return 0;
}

int BCRun::RunZ() {
    if(p_->domain_->klen_global_==1) {
        return 1;
    }
 
    global_profiler.Start(6);

    PeriodicZ();

    global_profiler.Pause(6);
    return 0;
}


int BCRun::PeriodicX() {
#pragma omp parallel for collapse(3)
    for(int n=0; n<p_->domain_->stencil_; ++n) {
        for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
            for(int j=p_->domain_->jhalobeg_; j<=p_->domain_->jhaloend_; ++j) {
        	    int i = p_->domain_->iend_+1+n;
                int idx0 = p_->flow_->rho_.GetIndex(i,                   k,j);
                int idx1 = p_->flow_->rho_.GetIndex(i-p_->domain_->ilen_,k,j);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
        
        	    i = p_->domain_->ibeg_-p_->domain_->stencil_+n;
                idx0 = p_->flow_->rho_.GetIndex(i,                   k,j);
                idx1 = p_->flow_->rho_.GetIndex(i+p_->domain_->ilen_,k,j);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
            }
        }
    }

    return 0;
}



int BCRun::PeriodicY() {
#pragma omp parallel for collapse(3)
    for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
        for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
            for(int n=0; n<p_->domain_->stencil_; ++n) {
        	    int j = p_->domain_->jend_+1+n;
                int idx0 = p_->flow_->rho_.GetIndex(i,k,j);
                int idx1 = p_->flow_->rho_.GetIndex(i,k,j-p_->domain_->jlen_);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
        
        	    j = p_->domain_->jbeg_-p_->domain_->stencil_+n;
                idx0 = p_->flow_->rho_.GetIndex(i,k,j);
                idx1 = p_->flow_->rho_.GetIndex(i,k,j+p_->domain_->jlen_);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
            }
        }
    }

    return 0;
}


int BCRun::PeriodicZ() {
#pragma omp parallel for collapse(3)
    for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
        for(int n=0; n<p_->domain_->stencil_; ++n) {
            for(int j=p_->domain_->jhalobeg_; j<=p_->domain_->jhaloend_; ++j) {
        	    int k = p_->domain_->kend_+1+n;
                int idx0 = p_->flow_->rho_.GetIndex(i,k,j);
                int idx1 = p_->flow_->rho_.GetIndex(i,k-p_->domain_->klen_,j);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
        
        	    k = p_->domain_->kbeg_-p_->domain_->stencil_+n;
                idx0 = p_->flow_->rho_.GetIndex(i,k,j);
                idx1 = p_->flow_->rho_.GetIndex(i,k+p_->domain_->klen_,j);
        	    p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
        	    p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
        	    p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
        	    p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
        	    p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
        	    p_->flow_->UpdateFlowFromConservative(idx0);
            }
        }
    }

    return 0;
}


int BCRun::ExtrapolateZerothXEnd() {
    for(int i=p_->domain_->iend_; i<=p_->domain_->ihaloend_; ++i) {
        for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
            for(int j=p_->domain_->jhalobeg_; j<=p_->domain_->jhaloend_; ++j) {
                int idx0 = p_->flow_->rhou_.GetIndex(i  ,k,j);
                int idx1 = p_->flow_->rhou_.GetIndex(i-1,k,j);
	            p_->flow_->  rho_.Get(idx0) = p_->flow_->  rho_.GetValue(idx1);
	            p_->flow_-> rhou_.Get(idx0) = p_->flow_-> rhou_.GetValue(idx1);
	            p_->flow_-> rhov_.Get(idx0) = p_->flow_-> rhov_.GetValue(idx1);
	            p_->flow_-> rhow_.Get(idx0) = p_->flow_-> rhow_.GetValue(idx1);
	            p_->flow_->rhoet_.Get(idx0) = p_->flow_->rhoet_.GetValue(idx1);
	            p_->flow_->UpdateFlowFromConservative(idx0);
         	}
        }
    }	
    return 0;
}


