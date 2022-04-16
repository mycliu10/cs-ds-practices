#pragma once
#include "BC.hpp"
#include "everything.hpp"
#include "central_diff.hpp"
#include "upwind.hpp"

class compute{
public:
    static int viscous_solve(FlowField & flow);
    static int convection_solve(FlowField & flow);
    static int DNS(FlowField & flow, BoundaryCondition & BC);
    static double ***fx00, ***fx01, **fx0, **fx1, **fx2, **fx3, **fx4;
    static double ***fy00, ***fy01, **fy0, **fy1, **fy2, **fy3, **fy4;
    static Array2D<double*> ptr2d_inpt, ptr2d_inpt_extra;
    static double **fflux_iextra;
    static double *pflux_iextra;
    static double ***f_iextra;
    static double **p_iextra;
    static double **u_iextra;
    static double **a_iextra;
    static double *M_iextra;
    static double **fflux_jextra;
    static double *pflux_jextra;
    static double ***f_jextra;
    static double **p_jextra;
    static double **u_jextra;
    static double **a_jextra;
    static double *M_jextra;



};

