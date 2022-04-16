#pragma once
#include <iostream>
#include "scheme.hpp"

using std::cout;
using std::endl;


class WENOFamily : public Scheme {
private:
    struct WENOCoef {
        CoefNote candidate;
        vector<CoefNote> smoothness;
        double weight;
    };
    vector<WENOCoef> WENOcoefs_;

public:
    WENOFamily(const int stencil, const int type) {
        stencil_ = stencil;

        switch(type) {
            case(0): {
                switch(stencil_) {
                    case(8): {
                        cout << "WENO7-JS initialized." << endl;
                        break;
                    }
                    case(6): {
                        stencil_ = 6;
                        offset_ = -2;
                        order_ = 5;

                        double beta2 = std::sqrt(13./12.);                
                        WENOcoefs_ = {
                            WENOCoef{
                                CoefNote({{-2,2./6.}, {-1,-7./6.}, {0,11./6.}}),
                                {
                                    CoefNote({{-2,1./2.}, {-1,-4./2.}, {0,3./2.}}),
                                    CoefNote({{-2,beta2}, {-1,-2.*beta2}, {0,beta2}}),
                                },
                                1./10.},
                            WENOCoef{
                                CoefNote({{-1,-1./6.}, {0,5./6.}, {1,2./6.}}),
                                {
                                    CoefNote({{-1,1./2.}, {1,-1./2.}}),
                                    CoefNote({{-1,beta2}, {0,-2.*beta2}, {1,beta2}}),
                                },
                                6./10.},
                            WENOCoef{
                                CoefNote({{0,2./6.}, {1,5./6.}, {2,-1./6.}}),
                                {
                                    CoefNote({{0,3./2.}, {1,-4./2.}, {2,1./2.}}),
                                    CoefNote({{0,beta2}, {1,-2.*beta2}, {2,beta2}}),
                                },
                                3./10.}};

                        cout << "WENO5-JS initialized." << endl;
                        break;
                    }
                    case(4): {
                        stencil_ = 4;
                        offset_ = -1;
                        order_ = 3;
                        
                        WENOcoefs_ = {
                            WENOCoef{
                                CoefNote({{-1,-1./2.}, {0,3./2.}}),
                                {
                                    CoefNote({{-1,1.}, {0,-1.}}),
                                },
                                1./3.},
                            WENOCoef{
                                CoefNote({{0,1./2.}, {1,1./2.}}),
                                {
                                    CoefNote({{0,1.}, {1,-1.}}),
                                },
                                2./3.}};

                        cout << "WENO3-JS initialized." << endl;
                        break; 
                    }
                };
            break;
            }
        }
        cout << "========================================================" << endl;
    }

    double Kernel(const vector<double>::iterator it) {
        vector<double> q (5, 0.);
        vector<double> si(5, 0.);
        vector<double> w (5, 0.);
        
        const double p = 2.;
        const double epsilon = 1.e-6;
    
        
        for(int k=0; k<WENOcoefs_.size(); ++k) {
            q[k] = WENOcoefs_[k].candidate.Calculate(it);
            for(auto& sm : WENOcoefs_[k].smoothness) {
                double s = sm.Calculate(it);
                si[k] += s*s;
            }
        }
        double alpha_sum = 0.;
        for(int k=0; k<WENOcoefs_.size(); ++k) {
            w[k] = WENOcoefs_[k].weight / std::pow(epsilon+si[k], p);
            alpha_sum += w[k];
        }
        for(int k=0; k<WENOcoefs_.size(); ++k) {
            w[k] /= alpha_sum;
        }
    
        double fbar=0.;
        for(int k=0; k<WENOcoefs_.size(); ++k) {
            fbar += w[k]*q[k];
        }
    
        return fbar;
    }
};



