#pragma once

class LocalWorker {
public:
    double * u_; 
    double * v_; 
    double * w_; 
    double * rho_; 
    double * p_; 
    double * T_; 
    double * et_; 
    double * a_; 
    double * upa_; 
    double * uma_; 
    double * rhoet_; 
    double * uu_; 
    double * cc_; 
    double * pp_; 
    double * fu_; 
    double * fp_; 
    double * fm_; 
    double * f_; 
    double * sx_; 
    double * sy_; 
    double * sz_; 
    int focus_;
    int stencil_;
    LocalWorker() {}

};
