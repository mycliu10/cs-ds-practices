#include "variable.hpp"
#include "ideal_gas.hpp"

variable::variable() {}
//:: _rho(1.), _rhou(1.), _rhov(0.), _E(1.), _u(1.), _v(0.), _p(1.), _T(1.), _mu(1.), _kappa(1.), _a(1.) {}

int variable::Get_everything_from_primitive(){
    int err;

    err = ideal_gas::Get_rho_and_a(p_, T_, rho_, a_);
    err = ideal_gas::Get_mu_Sutherland(T_, mu_);
    err = ideal_gas::Get_kappa(mu_, kappa_);
    E_ = ideal_gas::Cv*T_ + 0.5*(u_*u_+v_*v_);
    rhou_ = rho_*u_;
    rhov_ = rho_*v_;
    rhoE_ = rho_*E_;
    rhoh_ = rhoE_ + p_;
    rhouu_ = rhou_*u_;
    rhovv_ = rhov_*v_;
    rhouv_ = rhou_*v_;
    rhouE_ = rhoE_*u_;
    rhovE_ = rhoE_*v_;
    rhouh_ = rhoh_*u_;
    rhovh_ = rhoh_*v_;
    return err;
}
int variable::Get_everything(){
    int err;
    u_ = rhou_ / rho_;
    v_ = rhov_ / rho_;
    E_ = rhoE_ / rho_;
    T_ = (E_ - 0.5*(u_*u_ + v_*v_)) / ideal_gas::Cv;
    err = ideal_gas::Get_p_and_a(rho_, T_, p_, a_);
    err = ideal_gas::Get_mu_Sutherland(T_, mu_);
    err = ideal_gas::Get_kappa(mu_, kappa_);
    rhoh_ = rhoE_ + p_;
    rhouu_ = rhou_*u_;
    rhovv_ = rhov_*v_;
    rhouv_ = rhou_*v_;
    rhouE_ = rhoE_*u_;
    rhovE_ = rhoE_*v_;
    rhouh_ = rhoh_*u_;
    rhovh_ = rhoh_*v_;
    return err;
}


int variable::CopyAll(const variable & var){
    rho_ = var.rho_;
    u_ = var.u_;
    v_ = var.v_;
    p_ = var.p_;
    E_ = var.E_;
    T_ = var.T_;
    a_ = var.a_;
    mu_ = var.mu_;
    kappa_ = var.kappa_;
    rhou_ = var.rhou_;
    rhov_ = var.rhov_;
    rhoE_ = var.rhoE_;
    rhoh_ = var.rhoh_;
    rhouu_ = var.rhouu_;
    rhouv_ = var.rhouv_;
    rhovv_ = var.rhovv_;
    rhouE_ = var.rhouE_;
    rhovE_ = var.rhovE_;
    rhouh_ = var.rhouh_;
    rhovh_ = var.rhovh_;
    return 1;
}
