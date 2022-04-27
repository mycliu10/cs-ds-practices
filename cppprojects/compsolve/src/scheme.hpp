#pragma once
#include "array.hpp"

class Scheme {
public:
    int derivative_order_;
    int stencil_;
    int offset_;
    int order_;

    Scheme() {
        derivative_order_ = 0;
        stencil_ = 0;
        offset_ = 0;
        order_ = 0;
    }

    ~Scheme() {};

    virtual double Kernel(const vector<double>::iterator) = 0; 

    vector<double>::iterator Offset(vector<double>::iterator it) {
        return it + offset_;
    }

    vector<double>::iterator Center(vector<double>::iterator it) {
        return it - offset_;
    }
};


class CoefNote {
public:
    unordered_map<int,double> coefs_;

    // coefs: (index, weight) pair
    // index is started from first element
    CoefNote(unordered_map<int,double> coefs) {
        coefs_ = coefs;
    }

    double Calculate(const vector<double>::iterator it) {
        double f = 0.;
        for(auto& coef : coefs_) {
            double x = *(it+coef.first);
            double w = coef.second;
            f += x*w;
        }
        return f;
    }
};


