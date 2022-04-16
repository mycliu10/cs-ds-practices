#pragma once
#include <sstream>
#include <iomanip>
#include "problem.hpp"

class Post {
private:
    Problem * p_;
    int evaluate_CFL_steps_;
    int restart_steps_;
    int residual_steps_;

public:
    Post(Problem *const p) {
        p_ = p;
        evaluate_CFL_steps_ = 1;
        restart_steps_ = 1;
    }

    int Setup(int restart_steps, int residual_steps, int cfl_steps) {
        restart_steps_ = restart_steps;
        residual_steps_ = residual_steps;
        evaluate_CFL_steps_ = cfl_steps;
        return 0;
    }

    int Restart(const int count) {
        if(count % restart_steps_ != 0) {
            return 1;
        }

        global_profiler.Start(20);

        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << count;
        string s = "flowdata_" + ss.str() + ".h5";
        p_->flow_->WriteFlow(s.c_str());
        std::cout << std::fixed ;
        std::cout << "Restart point set for step " << std::right  << std::setw(8) << std::to_string(count);
        std::cout << std::scientific << std::setprecision(6);
        std::cout << " , at t = " << p_->flow_->time_ << std::endl;

        global_profiler.Pause(20);
        return 0; 
    };

    int Residual(const int count) {
        double res = 0.;
        if(count % residual_steps_ != 0) {
            return 1;
        }

        global_profiler.Start(16);

        res = p_->rhs_->GetNorm();
        cout << std::scientific << std::setprecision(6);
        cout << "Residual drho = " << res;
        cout << std::fixed;
        cout << " at step = " << std::right  << std::setw(8) << std::to_string(count);
        cout << std::scientific << std::setprecision(6);
        cout << " , at t = " << p_->flow_->time_ << endl;
        

        global_profiler.Pause(16);
        return 0;
    }

    double EvaluateCFL(const int count, const double dt, const double CFL) {
        if(count % evaluate_CFL_steps_ != 0) {
            return dt;
        }

        global_profiler.Start(17);

        int ibeg = p_->domain_->ibeg_;
        int jbeg = p_->domain_->jbeg_;
        int kbeg = p_->domain_->kbeg_;
        int iend = p_->domain_->iend_;
        int jend = p_->domain_->jend_;
        int kend = p_->domain_->kend_;

        double fctr = p_->gas_->gamma_ / p_->gas_->Pr_;

        double dtcfl = 1.e9;
        double dtconv = 1.e9;
        double dtvisc = 1.e9;
        for(int i=ibeg; i<=iend; ++i) {
            for(int k=kbeg; k<=kend; ++k) {
                for(int j=jbeg; j<=jend; ++j) {
                    int idx  = p_->flow_->u_.GetIndex(i,k,j);
                    double u = p_->flow_->u_.GetValue(idx);
                    double v = p_->flow_->v_.GetValue(idx);
                    double w = p_->flow_->w_.GetValue(idx);
                    double a = p_->flow_->a_.GetValue(idx);
                    double didx = p_->mesh_->di_dx_.GetValue(idx);
                    double djdy = p_->mesh_->dj_dy_.GetValue(idx);
                    double dkdz = p_->mesh_->dk_dz_.GetValue(idx);
                    dtcfl = 1. / ((std::abs(u)+a)*didx + (std::abs(v)+a)*djdy +(std::abs(w)+a)*dkdz);
                    if(dtcfl < dtconv) {
                        dtconv = dtcfl;
                    }

                    double rho = p_->flow_->rho_.GetValue(idx);
                    double mu  = p_->flow_-> mu_.GetValue(idx);
                    double didxsq = p_->mesh_->di_dx_sq_.GetValue(idx);
                    double djdysq = p_->mesh_->dj_dy_sq_.GetValue(idx);
                    double dkdzsq = p_->mesh_->dk_dz_sq_.GetValue(idx);
                    double dtcfl = rho / mu / (didxsq+djdysq+dkdzsq) / fctr; 
                    if(dtcfl < dtvisc) {
                        dtvisc = dtcfl;
                    }
                }
            }
        }

        dtcfl = std::min(dtvisc, dtconv);

        global_profiler.Pause(17);
        return dtcfl*CFL;
 
    };
};
