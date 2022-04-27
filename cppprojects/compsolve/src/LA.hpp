#pragma once

class LA {
public:
    static int GaussianElimination(double *const A, double *const b, double *const x, const int N, const int num_vars) {
        // check for first element not being zero
        if( A[0] == 0. ) {
            for(int i=1; i<N; ++i) {
                if( A[i*N] != 0. ) {
                    for(int j=0; j<N; ++j) {
                        double t = A[i*N+j];
                        A[i*N+j] = A[j];
                        A[j] = t;
                    }
                    break;
                }
            }
        }


        for(int norm = 0; norm < N-1; ++norm) {
            for(int i = norm + 1; i < N; ++i) {
                double multiplier = A[i*N+norm] / A[norm*N+norm];
                for(int j = norm; j < N; ++j) {
                   	A[i*N+j] -= A[norm*N+j] * multiplier;
                }
                for(int n=0; n<num_vars; ++n) {
                    b[i*num_vars+n] -= b[norm*num_vars+n] * multiplier;
                }
            }
        }

        for(int i=N-1; i>=0; --i) {
            for(int n=0; n<num_vars; ++n) {
                x[i*num_vars+n] = b[i*num_vars+n];
                for(int j=N-1; j>i; --j) {
                    x[i*num_vars+n] -= A[i*N+j] * x[j*num_vars+n];
                }
                x[i*num_vars+n] /= A[i*N+i];
            }
        } 

        return 0;
    }
};
