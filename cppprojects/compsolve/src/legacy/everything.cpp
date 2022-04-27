#include "everything.hpp"
#include "central_difference.hpp"

FlowField::FlowField():vars(), viscs(){}

int FlowField::reset_rhss(){
    int err = 1;
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            err *= rhss.Get(i,j).reset_du();
        }
    }
}


int FlowField::multiply_rhss(const double &f){
    int err = 1;
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            err *= rhss.Get(i,j).multiply_du(f);
        }
    }
}


int FlowField::update_vars(const double &f){
    int err = 1;
    variable * var;
    rhs * prhs;
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            var = &vars.Get(i,j);
            prhs = &rhss.Get(i,j);
            (*var).rho_ += f*(*prhs)._du[0];
            (*var).rhou_ += f*(*prhs)._du[1];
            (*var).rhov_ += f*(*prhs)._du[2];
            (*var).rhoE_ += f*(*prhs)._du[3];
            err = (*var).Get_everything();
        }
    }
}

int FlowField::SolveViscous() {
    double *u[3], *v[3], *T[3], *mu[3], *kappa[3], *u_x[3], *v_x[3], *T_x[3];
    ViscousPoint * pViscousPoint;
    FlowPoint * pFlowPoint;
    int err;
    
    // computational section //
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            for(int count=0; count<3; count++){
                pFlowPoint = &flow_points_.Get(i+count-1,j);
                u[count]     = &(*pFlowPoint).u_;
                v[count]     = &(*pFlowPoint).v_;
                T[count]     = &(*pFlowPoint).T_;
                mu[count]    = &(*pFlowPoint).mu_;
                kappa[count] = &(*pFlowPoint).kappa_;
            }
            pViscousPoint = &viscous_points_.Get(i,j);
            // feed data to viscous instance //
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(u,     (*pViscousPoint)._u_x); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(v,     (*pViscousPoint)._v_x); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(T,     (*pViscousPoint)._T_x); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(mu,    (*pViscousPoint)._mu_x); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(kappa, (*pViscousPoint)._kappa_x); 
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(u,     (*pViscousPoint)._u_xx);
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(v,     (*pViscousPoint)._v_xx);
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(T,     (*pViscousPoint)._T_xx);
        }
    }
    
    for(int i=domain::ibeg; i<domain::iend; i++){
        for(int j=domain::jbeg; j<domain::jend; j++){
            for(int count=0; count<3; count++){
                pFlowPoint = &flow_points_.Get(i,j+count-1);
                pViscousPoint = &viscous_points_.Get(i,j+count-1);
                u[count]     = &(*pFlowPoint).u_;
                v[count]     = &(*pFlowPoint).v_;
                T[count]     = &(*pFlowPoint).T_;
                mu[count]    = &(*pFlowPoint).mu_;
                kappa[count] = &(*pFlowPoint).kappa_;
                u_x[count]   = &(*pViscousPoint).u_x_;
                v_x[count]   = &(*pViscousPoint).v_x_;
                T_x[count]   = &(*pViscousPoint).T_x_;
            }
            pFlowPoint = &flow_points_.Get(i,j);
            pViscousPoint = &viscous_points_.Get(i,j);
            // feed data to viscous instance //
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(u,     (*pViscousPoint).u_y_); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(v,     (*pViscousPoint).v_y_); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(T,     (*pViscousPoint).T_y_); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(mu,    (*pViscousPoint).mu_y_); 
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(kappa, (*pViscousPoint).kappa_y_); 
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(u,     (*pViscousPoint).u_yy_);
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(v,     (*pViscousPoint).v_yy_);
            err = CentralDifferenceSecondOrder::DifferentiateAcc2nd(T,     (*pViscousPoint).T_yy_);
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(u_x,   (*pViscousPoint).u_xy_);
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(v_x,   (*pViscousPoint).v_xy_);
            err = CentralDifferenceFirstOrder ::DifferentiateAcc2nd(T_x,   (*pViscousPoint).T_xy_);
            // instance process fed data //
            err = (*pViscousPoint).curv_diff(mesh_points_.Get(i,j));
            err = (*pViscousPoint).GetVelocityDivergence();
            err = (*pViscousPoint).GetViscousTerm( (*pFlowPoint).u_, (*pFlowPoint).v_, (*pFlowPoint).mu_, (*pFlowPoint).kappa_);

        }
    }

    return err;

}


