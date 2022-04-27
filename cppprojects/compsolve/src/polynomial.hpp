#pragma once
#include "LA.hpp"

class Polynomial {
public:
    static int GetCoefficients(const double *const x, const double *const y, double *const c, const int order, const int num_vars) {
        const int max_size = 16;
        double A[max_size*max_size];
        double b[max_size*max_size];
        int N = order + 1;

        for(int i=0; i<N; ++i) {
            A[i*N] = 1.;
            for(int j=1; j<N; ++j) {
                A[i*N+j] = A[i*N+j-1] * x[i];
            }
            for(int n=0; n<num_vars; ++n) {
                b[i*num_vars+n] = y[i*num_vars+n];
            }
        }

        LA::GaussianElimination(A, b, c, N, num_vars);

        return 0;
    }


    static int GetValues(const double *const c, const double x, double *const y, const int order, const int num_vars) {
        for(int n=0; n<num_vars; ++n) {
            double sum = c[n];
            double l = 1.;
            for(int i=1; i<order+1; ++i) {
                l *= x;
                sum += c[i*num_vars+n] * l;
            }
            y[n] = sum;
        }
        
        return 0;
    }







        


};
