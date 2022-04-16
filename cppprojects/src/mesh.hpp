#pragma once
#include "array.hpp"
#include "H5wrapper.hpp"
#include "domain.hpp"
#include "scheme.hpp"

class Mesh {
private:
    Array<double> buffer_read_;
    Array<double> buffer_write_;
    Array<double> buffer_mesh_;
    Domain * domain_;
    Scheme* scheme_1st_;
    Scheme* scheme_2nd_;
    const int num_extra_points_to_extend_ = 2;
    int num_points_to_extend_;
public:
    Array<double> x_;
    Array<double> y_;
    Array<double> z_;
    Array<double> dx_di_; 
    Array<double> dx_dj_; 
    Array<double> dx_dk_; 
    Array<double> dy_di_; 
    Array<double> dy_dj_; 
    Array<double> dy_dk_; 
    Array<double> dz_di_; 
    Array<double> dz_dj_; 
    Array<double> dz_dk_;

    Array<double> di_dx_; 
    Array<double> di_dy_; 
    Array<double> di_dz_; 
    Array<double> dj_dx_; 
    Array<double> dj_dy_; 
    Array<double> dj_dz_; 
    Array<double> dk_dx_; 
    Array<double> dk_dy_; 
    Array<double> dk_dz_; 

    Array<double> di_dx_sq_; 
    Array<double> dj_dy_sq_; 
    Array<double> dk_dz_sq_; 
    Array<double> ddi_dx2_; 
    Array<double> ddj_dy2_; 
    Array<double> ddk_dz2_; 
    
    Mesh(Domain *const domain);
    int ReadMesh(const char *filename);
    int ExtendMesh();
    int CalculateMetrics();
    
};

