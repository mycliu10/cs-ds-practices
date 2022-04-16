#pragma once
#include "mesh_point.hpp"

class ConvectionPoint {
public:
    double rhou_x_, rhouu_x_, _rhouv_x_, rhouh_x_, p_x_; 
    double rhov_y_, rhouv_y_, _rhovv_y_, rhovh_y_, p_y_; 
    double convt_rho_, convt_rhou_, convt_rhov_, convt_rhoE_;
    
    int GetConvectionTerm();
    int CurvilinearDifferentiate(MeshPoint& msh);
};
