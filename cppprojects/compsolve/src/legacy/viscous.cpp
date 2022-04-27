#include "viscous.hpp"
#include "central_diff.hpp"

const double c4i3 = 4./3.;
const double c2i3 = 2./3.;
const double c1i3 = 1./3.;

ViscousPoint::ViscousPoint(){}

int ViscousPoint::GetViscousTerm(const double& u, const double& v, const double& mu, double& kappa){
    tauxx_novisc_ = u_x_ + u_x_ - c2i3*div_u_;
    tauyy_novisc_ = v_y_ + v_y_ - c2i3*div_u_;
    tauxy_novisc_ = u_y_ + v_x_;

    visct_rhou_ = mu_x_*tauxx_novisc_ + mu_y_*tauxy_novisc_
                + mu * (u_xx_+u_yy_ + c1i3*(u_xx_+v_xy_));
    visct_rhov_ = mu_x_*tauxy_novisc_ + mu_y_*tauyy_novisc_
                + mu * (v_xx_+v_yy_ + c1i3*(u_xy_+v_yy_));
    visct_rhoE_ = mu * (u_x_*tauxx_novisc_ + v_y_*tauyy_novisc_ + (u_y_+v_x_)*tauxy_novisc_)
                + u*visct_rhou_ + v*visct_rhov_
                + kappa_x_*T_x_ + kappa_y_*T_y_
                + kappa * (T_xx_ + _T_yy_);
    return 1;
}



int ViscousPoint::GetVelocityDivergence(){
    div_u_ = u_x_ + v_y_;
    return 1;
}


int ViscousPoint::CurvilinearDifferentiate(MeshPoint & msh){
    double u_x = _u_x;    
    double u_y = _u_y;    
    double u_xx = _u_xx;   
    double u_yy = _u_yy;   
    double u_xy = _u_xy; 
    double v_x = _v_x;    
    double v_y = _v_y;    
    double v_xx = _v_xx;   
    double v_yy = _v_yy;   
    double v_xy = _v_xy; 
    double T_x = _T_x;    
    double T_y = _T_y;    
    double T_xx = _T_xx;   
    double T_yy = _T_yy;   
    double T_xy = _T_xy; 
    double mu_x = _mu_x;
    double mu_y = _mu_y;
    double kappa_x = _kappa_x;
    double kappa_y = _kappa_y;

    _u_x = msh.DifferentiateFirstOrder(u_x, 0);
    _u_y = msh.DifferentiateFirstOrder(u_y, 1);
    _u_xx = msh.DifferentiateSecondOrder(u_x, u_xx, 0, 0);
    _u_yy = msh.DifferentiateSecondOrder(u_y, u_yy, 1, 1);
    _u_xy = msh.DifferentiateSecondOrder(u_x, u_xy, 0, 1);
    _v_x = msh.DifferentiateFirstOrder(v_x, 0);
    _v_y = msh.DifferentiateFirstOrder(v_y, 1);
    _v_xx = msh.DifferentiateSecondOrder(v_x, v_xx, 0, 0);
    _v_yy = msh.DifferentiateSecondOrder(v_y, v_yy, 1, 1);
    _v_xy = msh.DifferentiateSecondOrder(v_x, v_xy, 0, 1);
    _T_x = msh.DifferentiateFirstOrder(T_x, 0);
    _T_y = msh.DifferentiateFirstOrder(T_y, 1);
    _T_xx = msh.DifferentiateSecondOrder(T_x, T_xx, 0, 0);
    _T_yy = msh.DifferentiateSecondOrder(T_y, T_yy, 1, 1);
    _T_xy = msh.DifferentiateSecondOrder(T_x, T_xy, 0, 1);
    _mu_x = msh.DifferentiateFirstOrder(mu_x, 0);
    _mu_y = msh.DifferentiateFirstOrder(mu_y, 1);
    _kappa_x = msh.DifferentiateFirstOrder(kappa_x, 0);
    _kappa_y = msh.DifferentiateFirstOrder(kappa_y, 1);
}






    
