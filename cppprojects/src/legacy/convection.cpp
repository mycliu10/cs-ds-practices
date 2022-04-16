#include "convection.hpp"


int ConvectionPoint::GetConvectionTerm(){
    convt_rho_  = rhou_x_  + rhov_y_; 
    convt_rhou_ = rhouu_x_ + rhouv_y_ + p_x_; 
    convt_rhov_ = rhouv_x_ + rhovv_y_ + p_y_; 
    convt_rhoE_ = rhouh_x_ + rhovh_y_; 
    
    return 1;
}


int ConvectionPoint::CurvilinearDifferentiate(MeshPoint& msh){
    double rhou_x = _rhou_x;    
    double rhouu_x = _rhouu_x;    
    double rhouv_x = _rhouv_x;    
    double rhouh_x = _rhouh_x;    
    double p_x = _p_x;
    double rhov_y = _rhov_y;    
    double rhouv_y = _rhouv_y;    
    double rhovv_y = _rhovv_y;    
    double rhovh_y = _rhovh_y;    
    double p_y = _p_y;

    _rhou_x = msh.DifferentiateFirstOrder(rhou_x, 0);
    _rhouu_x = msh.DifferentiateFirstOrder(rhouu_x, 0);
    _rhouv_x = msh.DifferentiateFirstOrder(rhouv_x, 0);
    _rhouh_x = msh.DifferentiateFirstOrder(rhouh_x, 0);
    _p_x = msh.DifferentiateFirstOrder(p_x, 0);
    _rhov_y = msh.DifferentiateFirstOrder(rhov_y, 1);
    _rhouv_y = msh.DifferentiateFirstOrder(rhouv_y, 1);
    _rhovv_y = msh.DifferentiateFirstOrder(rhovv_y, 1);
    _rhovh_y = msh.DifferentiateFirstOrder(rhovh_y, 1);
    _p_y = msh.DifferentiateFirstOrder(p_y, 1);

}







