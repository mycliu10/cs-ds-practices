#pragma once
#include "scheme.hpp"

class CDFamily : public Scheme {
private:
    CoefNote coefs_;
 
public:
    CDFamily(const int derivative_order, const int type) : coefs_{{}} {
        derivative_order_ = derivative_order;

        switch(type) {
            case(2): {
                switch(derivative_order_) {
                    case(1): {
                        stencil_ = 3;
                        offset_ = -1;
                        order_ = 2;
                        
                        coefs_ = CoefNote({{-1,-.5}, {1,.5}});

                        cout << "1st order Central difference of 2nd order accuracy initialized." << endl;
                        break;
                    }
                    case(2): {
                        stencil_ = 3;
                        offset_ = -1;
                        order_ = 2;

                        coefs_ = CoefNote({{-1,1.}, {0,-2.}, {1,1.}});

                        cout << "2nd order Central difference of 2nd order accuracy initialized." << endl;
                        break;
                    }
                }
                break;
            }
        }
    }

    double Kernel(const vector<double>::iterator it) {
        return coefs_.Calculate(it);
    }
};

