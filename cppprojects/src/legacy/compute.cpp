#include "compute.hpp"
#include "BC.hpp"
#include "everything.hpp"

double ** compute::fflux_iextra;
double *  compute::pflux_iextra;
double *** compute::f_iextra;
double **  compute::p_iextra;
double **  compute::u_iextra;
double **  compute::a_iextra;
double *   compute::M_iextra;
double ** compute::fflux_jextra;
double *  compute::pflux_jextra;
double *** compute::f_jextra;
double **  compute::p_jextra;
double **  compute::u_jextra;
double **  compute::a_jextra;
double *   compute::M_jextra;


int compute::viscous_solve(FlowField & flow){
    double *u[3], *v[3], *T[3], *mu[3], *kappa[3], *u_x[3], *v_x[3], *T_x[3];
    viscous * visc;
    variable * var;
    int err;
    
    // computational section //
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            for(int count=0; count<3; count++){
                var = &flow.vars.Get(i+count-1,j);
                u[count] = &(*var).u_;
                v[count] = &(*var).v_;
                T[count] = &(*var).T_;
                mu[count] = &(*var).mu_;
                kappa[count] = &(*var).kappa_;
            }
            visc = &flow.viscs.Get(i,j);
            // feed data to viscous instance //
            err = central_diff_1st::diff_acc2(u, (*visc)._u_x); 
            err = central_diff_1st::diff_acc2(v, (*visc)._v_x); 
            err = central_diff_1st::diff_acc2(T, (*visc)._T_x); 
            err = central_diff_1st::diff_acc2(mu, (*visc)._mu_x); 
            err = central_diff_1st::diff_acc2(kappa, (*visc)._kappa_x); 
            err = central_diff_2nd::diff_acc2(u, (*visc)._u_xx);
            err = central_diff_2nd::diff_acc2(v, (*visc)._v_xx);
            err = central_diff_2nd::diff_acc2(T, (*visc)._T_xx);
        }
    }
    
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            for(int count=0; count<3; count++){
                var = &flow.vars.Get(i,j+count-1);
                visc = &flow.viscs.Get(i,j+count-1);
                u[count] = &(*var).u_;
                v[count] = &(*var).v_;
                T[count] = &(*var).T_;
                mu[count] = &(*var).mu_;
                kappa[count] = &(*var).kappa_;
                u_x[count] = &(*visc)._u_x;
                v_x[count] = &(*visc)._v_x;
                T_x[count] = &(*visc)._T_x;
            }
            var = &flow.vars.Get(i,j);
            visc = &flow.viscs.Get(i,j);
            // feed data to viscous instance //
            err = central_diff_1st::diff_acc2(u, (*visc)._u_y); 
            err = central_diff_1st::diff_acc2(v, (*visc)._v_y); 
            err = central_diff_1st::diff_acc2(T, (*visc)._T_y); 
            err = central_diff_1st::diff_acc2(mu, (*visc)._mu_y); 
            err = central_diff_1st::diff_acc2(kappa, (*visc)._kappa_y); 
            err = central_diff_2nd::diff_acc2(u, (*visc)._u_yy);
            err = central_diff_2nd::diff_acc2(v, (*visc)._v_yy);
            err = central_diff_2nd::diff_acc2(T, (*visc)._T_yy);
            err = central_diff_1st::diff_acc2(u_x, (*visc)._u_xy);
            err = central_diff_1st::diff_acc2(v_x, (*visc)._v_xy);
            err = central_diff_1st::diff_acc2(T_x, (*visc)._T_xy);
            // instance process fed data //
            err = (*visc).curv_diff(flow.meshs.Get(i,j));
            err = (*visc).Get_detu();
            err = (*visc).Get_viscous_term( (*var).u_, (*var).v_, (*var).mu_, (*var).kappa_);

        }
    }

    return err;

}


int compute::convection_solve(FlowField & flow){
    convection * conv;
    variable * var;
    int err;
    int count = 0;
    
    // computational section //
    for(int i=domain::ibeg; i<domain::iend; i++){
        count = 0;
        for(int j=domain::jbeg-1; j<domain::jend+1; j++){
            var = &flow.vars.Get(i,j);
            f_jextra[count][0] = &(*var).rhov_; 
            f_jextra[count][1] = &(*var).rhouv_; 
            f_jextra[count][2] = &(*var).rhovv_; 
            f_jextra[count][3] = &(*var).rhovh_; 
            p_jextra[count] = &(*var).p_;
            u_jextra[count] = &(*var).v_;
            a_jextra[count] = &(*var).a_;
            count++;
        }
        // calculate flux //
        err = upwind::Get_flux_line(f_jextra, p_jextra, u_jextra, a_jextra, fflux_jextra, pflux_jextra, M_jextra, domain::jnpt+2, 4);
        // feed data to convection instance //
        count = 0;
        for(int j=domain::jbeg; j<domain::jend; j++){
            conv = &flow.convs.Get(i,j);
            (*conv)._rhov_y = fflux_jextra[count+1][0] - fflux_jextra[count][0];
            (*conv)._rhouv_y = fflux_jextra[count+1][1] - fflux_jextra[count][1];
            (*conv)._rhovv_y = fflux_jextra[count+1][2] - fflux_jextra[count][2];
            (*conv)._rhovh_y = fflux_jextra[count+1][3] - fflux_jextra[count][3];
            (*conv)._p_y = pflux_jextra[count+1] - pflux_jextra[count];
            count++;
        }
    }
    
    for(int j=domain::jbeg; j<domain::jend; j++){
        count = 0;
        for(int i=domain::ibeg-1; i<domain::iend+1; i++){
            var = &flow.vars.Get(i,j);
            f_iextra[count][0] = &(*var).rhou_; 
            f_iextra[count][1] = &(*var).rhouu_; 
            f_iextra[count][2] = &(*var).rhouv_; 
            f_iextra[count][3] = &(*var).rhouh_; 
            p_iextra[count] = &(*var).p_;
            u_iextra[count] = &(*var).u_;
            a_iextra[count] = &(*var).a_;
            count++;
        }
        // calculate flux //
        err = upwind::Get_flux_line(f_iextra, p_iextra, u_iextra, a_iextra, fflux_iextra, pflux_iextra, M_iextra, domain::inpt+2, 4);
        // feed data to convection instance //
        count = 0;
        for(int i=domain::ibeg; i<domain::iend; i++){
            conv = &flow.convs.Get(i,j);
            (*conv)._rhou_x = fflux_iextra[count+1][0] - fflux_iextra[count][0];
            (*conv)._rhouu_x = fflux_iextra[count+1][1] - fflux_iextra[count][1];
            (*conv)._rhouv_x = fflux_iextra[count+1][2] - fflux_iextra[count][2];
            (*conv)._rhouh_x = fflux_iextra[count+1][3] - fflux_iextra[count][3];
            (*conv)._p_x = pflux_iextra[count+1] - pflux_iextra[count];
            count++;
            // instance process fed data //
            (*conv).curv_diff(flow.meshs.Get(i,j));
            (*conv).Get_convection_term();
        }
    }
    return err;

}


int compute::DNS(FlowField & flow, BoundaryCondition & BC){
    int err;
     
    err = BC.BCI_.ForceEigenInflow(flow);
    err = BC.BCI_.ExtrapolateZerothOrderEnd(flow);
    err = BC.BCJ_.Periodic(flow);

    err = convection_solve(flow);
//    err = viscous_solve(flow);

    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            err = flow.rhss.Get(i,j).DNS_du(flow.convs.Get(i,j), flow.viscs.Get(i,j));
        }
    }
    return err;
}



