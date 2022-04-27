#pragma once

class upwind{
public:

//static int Get_flux_line(double ***f, double *p[], double *u[], double *a[], double ***fflux, double *pflux[], double * M[], //double * Mhalf[],
static int Get_flux_line(double ***f, double **p, double **u, double **a, double **fflux, double *pflux, double * M, //double * Mhalf[],
                          const int num_point, const int num_var);
 
};

