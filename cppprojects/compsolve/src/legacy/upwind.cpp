#include "upwind.hpp"
#include <cmath>
#include <iostream>


int upwind::Get_flux_line(double ***f, double **p, double **u, double **a, double **fflux, double *pflux, double *M, //double * Mhalf[],
                          const int num_point, const int num_var){
    double Mhalf;
    for(int n=0; n<num_point; n++){
        M[n] = *u[n] / *a[n];
    }

    for(int n=0; n<num_point-1; n++){
        if(M[n] > 1.){
            for(int m=0; m<num_var; m++){
                fflux[n][m] = *f[n][m];
            }
            pflux[n] = *p[n];
        }
        else if(M[n] < -1.){
            for(int m=0; m<num_var; m++){
                fflux[n][m] = *f[n+1][m];
            }
            pflux[n] = *p[n+1];
        }
        else{
            Mhalf = 0.25 * ( std::pow(M[n]+1., 2) - std::pow(M[n+1]-1., 2) );
            for(int m=0; m<num_var; m++){
                fflux[n][m] = Mhalf*0.5*(*f[n][m]+*f[n+1][m]) - 0.5*std::abs(Mhalf)*(*f[n+1][m]-*f[n][m]);
            }
            pflux[n] = 0.5 * ( *p[n]*(1.+M[n]) + *p[n+1]*(1.-M[n+1]) );
        }
    }

    return 1;
}
