#pragma once
#include "array.hpp"
#include "H5wrapper.hpp"
#include "domain.hpp"
#include "ideal_gas.hpp"

class Flow {
private:
    IdealGas * gas_;
    Domain * domain_;

    Array<double> buffer_;
public:
    double time_;
    Array<double> u_, v_, w_, p_, T_, et_, a_, mu_, kappa_; // et = total energy per density
    Array<double> rho_, rhou_, rhov_, rhow_, rhoet_;// rhoh_; // Q
//    Array<double> rhouu_, rhouv_, rhouw_, rhouht_;   // F      H = total enthalpy per density
//    Array<double> rhovv_, rhovw_, rhovht_;           // G 
//    Array<double> rhoww_, rhowht_;                   // H
   
    Flow(IdealGas *const gas, Domain *const domain);
    int ReadFlow(const char *filename);
    int WriteFlow(const char *filename);

    inline int UpdateFlowFromPrimitive(const int i) {
        double T = T_.GetValue(i);
        rho_.Get(i) = p_.GetValue(i) / (gas_->R_ * T);

        double rho = rho_.GetValue(i);
        double u = u_.GetValue(i);
        double v = v_.GetValue(i);
        double w = w_.GetValue(i);
        double KE = 0.5 * (u*u+v*v+w*w);
        et_.Get(i) = gas_->Cv_*T + KE;

         rhou_.Get(i) = u * rho;
         rhov_.Get(i) = v * rho;
         rhow_.Get(i) = w * rho;
        rhoet_.Get(i) = et_.GetValue(i) * rho;
            a_.Get(i) = std::sqrt(gas_->gamma_*gas_->R_*T);
           mu_.Get(i) = gas_->GetDynamicViscositySutherland(T);
        kappa_.Get(i) = gas_->GetThermalConductivity(mu_.GetValue(i));
        
        return 0;
    }
    inline int UpdateFlowFromConservative(const int i) {
        double rho = rho_.GetValue(i);
         u_.Get(i) =  rhou_.GetValue(i) / rho;
         v_.Get(i) =  rhov_.GetValue(i) / rho;
         w_.Get(i) =  rhow_.GetValue(i) / rho;
        et_.Get(i) = rhoet_.GetValue(i) / rho;
        
        double u = u_.GetValue(i);
        double v = v_.GetValue(i);
        double w = w_.GetValue(i);
        double KE = 0.5 * (u*u+v*v+w*w);
        T_.Get(i) = (et_.GetValue(i) - KE) / gas_->Cv_; 
        
        double T = T_.GetValue(i);
            p_.Get(i) = rho * gas_->R_ * T;
            a_.Get(i) = std::sqrt(gas_->gamma_*gas_->R_*T);
           mu_.Get(i) = gas_->GetDynamicViscositySutherland(T);
        kappa_.Get(i) = gas_->GetThermalConductivity(mu_.GetValue(i));
        return 0;
    }




    inline static int UpdateFlowFromPrimitive(Flow &flow, const IdealGas &gas, const int i) {
        double T = flow.T_.GetValue(i);
        flow.  rho_.Get(i) = flow.p_.GetValue(i) / (gas.R_ * T);

        double rho = flow.rho_.GetValue(i);
        double u = flow.u_.GetValue(i);
        double v = flow.v_.GetValue(i);
        double w = flow.w_.GetValue(i);
        double KE = 0.5 * (u*u+v*v+w*w);
        flow.   et_.Get(i) = gas.Cv_*T + KE;

        flow. rhou_.Get(i) = u * rho;
        flow. rhov_.Get(i) = v * rho;
        flow. rhow_.Get(i) = w * rho;
        flow.rhoet_.Get(i) = flow.et_.GetValue(i) * rho;
        flow.    a_.Get(i) = std::sqrt(gas.gamma_*gas.R_*T);
        flow.   mu_.Get(i) = gas.GetDynamicViscositySutherland(T);
        flow.kappa_.Get(i) = gas.GetThermalConductivity(flow.mu_.GetValue(i));
        
        return 0;
    }

    inline static int UpdateFlowFromConservative(Flow &flow, const IdealGas &gas, const int i) {
        double rho = flow.rho_.GetValue(i);
        flow. u_.Get(i) = flow. rhou_.GetValue(i) / rho;
        flow. v_.Get(i) = flow. rhov_.GetValue(i) / rho;
        flow. w_.Get(i) = flow. rhow_.GetValue(i) / rho;
        flow.et_.Get(i) = flow.rhoet_.GetValue(i) / rho;
        
        double u = flow.u_.GetValue(i);
        double v = flow.v_.GetValue(i);
        double w = flow.w_.GetValue(i);
        double KE = 0.5 * (u*u+v*v+w*w);
        flow.T_.Get(i) = (flow.et_.GetValue(i) - KE) / gas.Cv_; 
        
        double T = flow.T_.GetValue(i);
        flow.    p_.Get(i) = rho * gas.R_ * T;
        flow.    a_.Get(i) = std::sqrt(gas.gamma_*gas.R_*T);
        flow.   mu_.Get(i) = gas.GetDynamicViscositySutherland(T);
        flow.kappa_.Get(i) = gas.GetThermalConductivity(flow.mu_.GetValue(i));
        return 0;
    }

};
