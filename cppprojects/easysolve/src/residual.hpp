#pragma once
#include "head.hpp"

class Residual {
    double l1;
    double l2;
    double lmax;

public:
    Residual() {
        reset();
    }

    void reset() {
        l1 = 0.;
        l2 = 0.;
        lmax = 0.;
    }

    void compute(double f) {
        f = std::abs(f);
        l1 += f;
        l2 += f * f;
        lmax = std::max(lmax, l1);
    }

    std::stringstream report() {
        std::stringstream stream;
        stream.setf(std::ios::scientific);
        stream.precision(2);
        stream << " L1 / L2 / Lmax residual = " 
                << l1 << " / "
                << l2 << " / "
                << lmax;
        return stream;
    }

    double getLmaxResidual() {
        return lmax;
    }
};