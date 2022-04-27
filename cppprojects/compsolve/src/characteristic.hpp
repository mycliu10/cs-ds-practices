#pragma once
#include <iostream>
#include <array>
#include "ideal_gas.hpp"
#include "worker.hpp"

using std::array;

class Transformer {
private:
    double gamma_;

    template <class It>
    int CharacteristicToPhysicalCartesian(const double u, const double v, const double w, 
                                          const double a, 
                                          const double sx, const double sy, const double sz,
                                          const It it_c,
                                          It it_p) {
        double gamma_minus_one_ = gamma_ - 1.;
        double K = .5 * (u*u+v*v+w*w);
        double H = a*a/gamma_minus_one_ + K;
        double u_star = u*sx + v*sy + w*sz;
        double r21 = u - a*sx;
        double r31 = v - a*sy;
        double r41 = w - a*sz;
        double r25 = u + a*sx;
        double r35 = v + a*sy;
        double r45 = w + a*sz;
        double r51 = H - u_star*a;
        double r55 = H + u_star*a;
        double r52 = u*sy + v*sz + w*sx;
        double r53 = u*sz + v*sx + w*sy;
        
        *(it_p  ) =     (*(it_c))                                         +   (*(it_c+3)) +     (*(it_c+4));
        *(it_p+1) = r21*(*(it_c)) +  sy*(*(it_c+1)) +  sz*(*(it_c+2)) + u*(*(it_c+3)) + r25*(*(it_c+4));
        *(it_p+2) = r31*(*(it_c)) +  sz*(*(it_c+1)) +  sx*(*(it_c+2)) + v*(*(it_c+3)) + r35*(*(it_c+4));
        *(it_p+3) = r41*(*(it_c)) +  sx*(*(it_c+1)) +  sy*(*(it_c+2)) + w*(*(it_c+3)) + r45*(*(it_c+4));
        *(it_p+4) = r51*(*(it_c)) + r52*(*(it_c+1)) + r53*(*(it_c+2)) + K*(*(it_c+3)) + r55*(*(it_c+4));
    
        return 0;
    }


    template <class It>
    int PhysicalToCharacteristicCartesian(const double u, const double v, const double w, 
                                          const double a, 
                                          const double sx, const double sy, const double sz,
                                          const It it_p,
                                          It it_c) {
        double gamma_minus_one_ = gamma_ - 1.;
        double K = .5 * (u*u+v*v+w*w);
        double u_star = u*sx + v*sy + w*sz;
        double one_over_two_a = 1. / (2.*a);
        double r55 = gamma_minus_one_ / (2.*a*a);
        double r11 = u_star*one_over_two_a + r55*K;
        double r12 = -sx*one_over_two_a - r55*u;
        double r13 = -sy*one_over_two_a - r55*v;
        double r14 = -sz*one_over_two_a - r55*w;
        double r31 = - (u*sz+v*sx+w*sy);
        double r21 = -(u+v+w) - r31 + u_star;
        double r22 = 1. - sz - sx;
        double r23 = 1. - sx - sy;
        double r24 = 1. - sy - sz;
        double r41 = 1. - 2.*r55*K;
        double r42 = 2.*r55*u;
        double r43 = 2.*r55*v;
        double r44 = 2.*r55*w;
        double r45 = -2.*r55;
        double r51 = -u_star*one_over_two_a + r55*K;
        double r52 = sx*one_over_two_a - r55*u;
        double r53 = sy*one_over_two_a - r55*v;
        double r54 = sz*one_over_two_a - r55*w;
    
        *(it_c  ) = r11*(*it_p) + r12*(*(it_p+1)) + r13*(*(it_p+2)) + r14*(*(it_p+3)) + r55*(*(it_p+4));
        *(it_c+1) = r21*(*it_p) + r22*(*(it_p+1)) + r23*(*(it_p+2)) + r24*(*(it_p+3));              
        *(it_c+2) = r31*(*it_p) +  sz*(*(it_p+1)) +  sx*(*(it_p+2)) +  sy*(*(it_p+3));              
        *(it_c+3) = r41*(*it_p) + r42*(*(it_p+1)) + r43*(*(it_p+2)) + r44*(*(it_p+3)) + r45*(*(it_p+4));
        *(it_c+4) = r51*(*it_p) + r52*(*(it_p+1)) + r53*(*(it_p+2)) + r54*(*(it_p+3)) + r55*(*(it_p+4));
    
        return 0;
    }   

public:
    Transformer() {
        gamma_ = 1.4;
        std::cout << "Transformer created." << std::endl;
        std::cout << "========================================================" << std::endl;
    }

    template <class It>
    inline int CharacteristicToPhysicalCartesian(const double u, const double v, const double w, 
                                          	     const double a, 
												 const int direction,
                                          	     const It it_c,
                                          	     It it_p) {
        double sx = 1.;
        double sy = 0.;
        double sz = 0.;
        switch(direction) {
            case(0): {
                break;
            }
            case(1): {
                sx = 0.;
                sy = 1.;
                sz = 0.;
                break;
            }
            case(2): {
                sx = 0.;
                sy = 0.;
                sz = 1.;
                break;
            }
        }

        int err = CharacteristicToPhysicalCartesian(u, v, w, a,
                                                    sx, sy, sz, it_c, it_p);
    
        return err;
    }
 
    template <class It>
    inline int PhysicalToCharacteristicCartesian(const double u, const double v, const double w, 
                                          		 const double a, 
										  		 const int direction,
                                          		 const It it_p,
                                          		 It it_c) {
        double sx = 1.;
        double sy = 0.;
        double sz = 0.;
        switch(direction) {
            case(0):
                break;
            case(1):
                sx = 0.;
                sy = 1.;
                sz = 0.;
                break;
            case(2):
                sx = 0.;
                sy = 0.;
                sz = 1.;
                break;
        }

        int err = PhysicalToCharacteristicCartesian(u, v, w, a,
                                                    sx, sy, sz, it_p, it_c);
    
        return err;
    }
};


