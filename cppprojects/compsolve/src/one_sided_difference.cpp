#include "one_sided_difference.hpp"

OSDFamily::OSDFamily() {
    derivative_order_ = 1;
    order_of_accuracy_ = 2;
    stencil_ = 2;
    for(int n=0; n<8; ++n) {
        coefs_[n] = 0.;
    }
}

double OSDFamily::Kernel(double *const f) {
    double df = 0.;
    for(int n=0; n<stencil_; ++n) {
        df += coefs_[n] * f[n];
    }
    
    return df;
}



OSD1stFamily::OSD1stFamily(const int order_of_accuracy) {
    derivative_order_ = 1;
    order_of_accuracy_ = order_of_accuracy;
    switch(order_of_accuracy_) {
        case(1):
            stencil_ = 2;
            coefs_[0] = -1.;
            coefs_[1] =  1.;
            break;
        case(2):
            stencil_ = 3;
            coefs_[0] = -3./2.;
            coefs_[1] =  4./2.;
            coefs_[2] = -1./2.;
            break;
        case(3):
            stencil_ = 4;
            coefs_[0] = -11./6.;
            coefs_[1] =  18./6.;
            coefs_[2] = - 9./6.;
            coefs_[3] =   2./6.;
            break;
        case(4):
            stencil_ = 5;
            coefs_[0] = -25./12.;
            coefs_[1] =  48./12.;
            coefs_[2] = -36./12.;
            coefs_[3] =  16./12.;
            coefs_[4] = - 3./12.;
            break;
    }
}

OSD2ndFamily::OSD2ndFamily(const int order_of_accuracy) {
    derivative_order_ = 2;
    order_of_accuracy_ = order_of_accuracy;
    switch(order_of_accuracy_) {
        case(1):
            stencil_ = 3;
            coefs_[0] =  1.;
            coefs_[1] = -2.;
            coefs_[2] =  1.;
            break;
        case(2):
            stencil_ = 4;
            coefs_[0] =  2.;
            coefs_[1] = -5.;
            coefs_[2] =  4.;
            coefs_[3] = -1.;
            break;
        case(3):
            stencil_ = 5;
            coefs_[0] =   35./12.;
            coefs_[1] = -104./12.;
            coefs_[2] =  114./12.;
            coefs_[3] = - 56./12.;
            coefs_[4] =   11./12.;
            break;
        case(4):
            stencil_ = 6;
            coefs_[0] =   45./12.;
            coefs_[1] = -154./12.;
            coefs_[2] =  214./12.;
            coefs_[3] = -156./12.;
            coefs_[4] =   61./12.;
            coefs_[5] = - 10./12.;
            break;
    }
}
