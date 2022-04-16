#pragma once
#include "mesh_point.hpp"

class Viscous {
public:
    Array2d<double> u_x_, u_y_, u_xx_, u_yy_, u_xy_;
    Array2d<double> v_x_, v_y_, v_xx_, v_yy_, v_xy_;
    Array2d<double> T_x_, T_y_, T_xx_, T_yy_, T_xy_;
    Array2d<double> div_u_;
    Array2d<double> mu_x_, mu_y_;
    Array2d<double> kappa_x_, _kappa_y_;
    Array2d<double> visct_rhou_, visct_rhov_, visct_rhoE_;
    Array2d<double> tauxx_novisc_, tauyy_novisc_, tauxy_novisc_;
    
    Viscous();

    int GetViscousTerm(const double & u, const double & v, const double & mu, double & kappa);
    int Get_viscous_ddx(double * u[], double * v[], double * T[], double * mu[], double * kappa[]);
    int Get_viscous_ddy(double * u[], double * v[], double * T[], double * mu[], double * kappa[]);
    int GetVelocityDivergence();
    int CurvilinearDifferentiate(MeshPoint & msh);

};

