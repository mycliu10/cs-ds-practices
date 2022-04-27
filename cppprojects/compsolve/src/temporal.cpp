#include "temporal.hpp"

int TimeIntegrator::Setup(DNSRun *const run, Post *const post, const double dt, const double CFL) {
    run_ = run;
    post_ = post;
    dt_ = dt;
    CFL_ = CFL;

    cout << std::scientific << std::setprecision(2);
    cout << "Setup: dt = " << dt_ << " , CFL = " << CFL_ << endl;
    
    return 0;
}


int TimeIntegrator::SetTimeStep(const double dt, const int count) {
    if(dt < dt_) {
        double dt_old = dt_;
        dt_ = dt;
        double dt_rat = dt_/dt_old;
        double dt_inc = dt_rat - 1.;
        cout << std::scientific << std::setprecision(2);
        cout << "Updated dt = " << dt_;
        cout << std::fixed << std::setprecision(2) << std::showpos;
        cout << " s (" << dt_inc*100. << "%)";
        cout << std::fixed << std::setprecision(2) << std::noshowpos;
        cout << " at CFL = " << CFL_;
        cout << " at step " << std::right  << std::setw(8) << std::to_string(count) << endl;
        return 1;
    }

    return 0;
}

int TimeIntegrator::Run(const int start_step, const int num_steps) {
    global_profiler.Disenable();
    start_step_ = start_step;
    // pre run
    run_->EvaluateRHS();
    double dt = post_->EvaluateCFL(0, dt_, CFL_);
    SetTimeStep(dt, start_step_+1);
    global_profiler.Enable();
 
    // start
    global_profiler.Start(0);
    for(int n=start_step_+1; n<num_steps+start_step_+1; ++n) {
        double dt = post_->EvaluateCFL(n, dt_, CFL_);
        SetTimeStep(dt, n);
 
        Kernel();

        post_->Residual(n);
        post_->Restart(n);

    }

    global_profiler.Pause(0);
    return 0;
}


int Euler::Kernel() {
    run_->EvaluateRHS();
    run_->Integrate(dt_, dt_);
    return 0;
}

int LowStorageRK3::Kernel() {
    const double dts [3] = { 1./3.,   15./16. , 8./15.};
    const double dtp [3] = { 1./3.,    5./12. , 1./ 4.};
    const double fctr[3] = {-5./9., -153./128., 1.};

    run_->ScaleRHS(0.);
 
    run_->EvaluateRHS();
    run_->Integrate(dt_*dts[0], dt_*dtp[0]);

    run_->ScaleRHS(fctr[0]);
    run_->EvaluateRHS();
    run_->Integrate(dt_*dts[1], dt_*dtp[1]);

    run_->ScaleRHS(fctr[1]);
    run_->EvaluateRHS();
    run_->Integrate(dt_*dts[2], dt_*dtp[2]);

    return 0; 
}


int Temporal::Kernel(const Domain &domain, Flow &flow, const RHS &rhs, const IdealGas &gas, const double dt) {
    int ibeg = domain.ibeg_;
    if(domain.ibegbc_==1) {
	    ibeg = domain.ibeg_+1;
    }

    for(int i=ibeg; i<=domain.iend_; ++i) {
        for(int k=domain.kbeg_; k<=domain.kend_; ++k) {
            for(int j=domain.jbeg_; j<=domain.jend_; ++j) {
                int idx  = flow.rho_.GetIndex(k,i,j);
                int idx4 = rhs . dQ_.GetIndex(k,i,j,0);
                flow.  rho_.Get(idx) += rhs.dQ_.GetValue(idx4  )*dt;
                flow. rhou_.Get(idx) += rhs.dQ_.GetValue(idx4+1)*dt;
                flow. rhov_.Get(idx) += rhs.dQ_.GetValue(idx4+2)*dt;
                flow. rhow_.Get(idx) += rhs.dQ_.GetValue(idx4+3)*dt;
                flow.rhoet_.Get(idx) += rhs.dQ_.GetValue(idx4+4)*dt;
   	            Flow::UpdateFlowFromConservative(flow, gas, idx);
            }
        }
   }
   return 0;
}



