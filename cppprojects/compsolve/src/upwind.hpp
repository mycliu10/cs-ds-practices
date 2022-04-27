#pragma once
#include "scheme.hpp"

class UpwindFamily : public Scheme {
protected:

public:
    UpwindFamily(const int order) {
        stencil_ = 3;
        offset_ = -1;
        order_ = order;
    }

    double Kernel(const double *const f) {
        double df = f[1] - f[0];
        return df;
    }

    double Kernel(const double *const f, const double a) {
        double df = 0.;
        if(a>=0.) {
            df = f[1] - f[0];
        }
        else {
            df = f[2] - f[1];
        }

        return df;
    }

};


