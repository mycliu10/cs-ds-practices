#pragma once
#include <iostream>
#include <cmath>
#include "ideal_gas.hpp"
#include "worker.hpp"

class Averager {
protected:
    double gamma_;
public:
    Averager() {
        gamma_ = 1.4;
    }

    virtual ~Averager() {};
    virtual int Kernel(const double rhol,   const double rhor, 
               const double ul,     const double ur,
               const double vl,     const double vr,
               const double wl,     const double wr,
               const double pl,     const double pr,
               const double rhoetl, const double rhoetr,
               double &uhat, double &vhat, double &what, double &ahat) = 0;
};


class RoeAverager : public Averager {
public:
    RoeAverager() {
        std::cout << "RoeAverager created." << std::endl;
        std::cout << "========================================================" << std::endl;
    }

    int Kernel(const double rhol,   const double rhor, 
               const double ul,     const double ur,
               const double vl,     const double vr,
               const double wl,     const double wr,
               const double pl,     const double pr,
               const double rhoetl, const double rhoetr,
               double &uhat, double &vhat, double &what, double &ahat) {
        double rrat = std::sqrt(rhor/rhol);
        double rri = 1. / (1. + rrat);
        uhat = (rrat * ur + ul) * rri;
        vhat = (rrat * vr + vl) * rri;
        what = (rrat * wr + wl) * rri;
        double Hhat = (rrat * (rhoetr+ pr)/rhor + (rhoetl+pl)/rhol) * rri;

        double Khat = .5 * (uhat*uhat+vhat*vhat+what*what);
        double ahat_sq = (gamma_-1.) * (Hhat - Khat);
        ahat = std::sqrt(ahat_sq);

        return 0;
    }

}; 

