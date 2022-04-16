#include "visc.hpp"

const static int BUFFER_SIZE = 1024;


int ViscousRun::RunY() {
    if(p_->domain_->jlen_global_==1) {
        return 1;
    }

    for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
        for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> T       (BUFFER_SIZE, 0.);
            vector<double> mu      (BUFFER_SIZE, 0.);
            vector<double> kappa   (BUFFER_SIZE, 0.);
            for(int j=p_->domain_->jhalobeg_; j<=p_->domain_->jhaloend_; ++j) {
                u    [j] = p_->flow_->u_    .GetValue(i,k,j);
                v    [j] = p_->flow_->v_    .GetValue(i,k,j);
                w    [j] = p_->flow_->w_    .GetValue(i,k,j);
                T    [j] = p_->flow_->T_    .GetValue(i,k,j);
                mu   [j] = p_->flow_->mu_   .GetValue(i,k,j);
                kappa[j] = p_->flow_->kappa_.GetValue(i,k,j);
            }
 
            for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
                u_j_ .Get(i,k,j) = scheme_1st_->Kernel(u.begin()+j);
                u_jj_.Get(i,k,j) = scheme_2nd_->Kernel(u.begin()+j);

                v_j_ .Get(i,k,j) = scheme_1st_->Kernel(v.begin()+j);
                v_jj_.Get(i,k,j) = scheme_2nd_->Kernel(v.begin()+j);

                w_j_ .Get(i,k,j) = scheme_1st_->Kernel(w.begin()+j);
                w_jj_.Get(i,k,j) = scheme_2nd_->Kernel(w.begin()+j);

                T_j_ .Get(i,k,j) = scheme_1st_->Kernel(T.begin()+j);
                T_jj_.Get(i,k,j) = scheme_2nd_->Kernel(T.begin()+j);

                mu_j_   .Get(i,k,j) = scheme_1st_->Kernel(mu.begin()+j);
                kappa_j_.Get(i,k,j) = scheme_1st_->Kernel(kappa.begin()+j);
            }
        }
    }

    return 0;
}


int ViscousRun::RunZ() {
    if(p_->domain_->klen_global_==1) {
        return 1;
    }

    for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
        for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> uj      (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> vj      (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> wj      (BUFFER_SIZE, 0.);
            vector<double> T       (BUFFER_SIZE, 0.);
            vector<double> mu      (BUFFER_SIZE, 0.);
            vector<double> kappa   (BUFFER_SIZE, 0.);
 
            for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
                u    [k] = p_->flow_->u_    .GetValue(i,k,j);
                v    [k] = p_->flow_->v_    .GetValue(i,k,j);
                w    [k] = p_->flow_->w_    .GetValue(i,k,j);
                T    [k] = p_->flow_->T_    .GetValue(i,k,j);
                mu   [k] = p_->flow_->mu_   .GetValue(i,k,j);
                kappa[k] = p_->flow_->kappa_.GetValue(i,k,j);
                uj   [k] = u_j_             .GetValue(i,k,j);
                vj   [k] = v_j_             .GetValue(i,k,j);
                wj   [k] = w_j_             .GetValue(i,k,j);
            }
            for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
                u_k_ .Get(i,k,j) = scheme_1st_->Kernel(u.begin()+k);
                u_kk_.Get(i,k,j) = scheme_2nd_->Kernel(u.begin()+k);

                v_k_ .Get(i,k,j) = scheme_1st_->Kernel(v.begin()+k);
                v_kk_.Get(i,k,j) = scheme_2nd_->Kernel(v.begin()+k);
                v_jk_.Get(i,k,j) = scheme_1st_->Kernel(vj.begin()+k);

                w_k_ .Get(i,k,j) = scheme_1st_->Kernel(w.begin()+k);
                w_kk_.Get(i,k,j) = scheme_2nd_->Kernel(w.begin()+k);
                w_jk_.Get(i,k,j) = scheme_1st_->Kernel(wj.begin()+k);

                T_k_ .Get(i,k,j) = scheme_1st_->Kernel(T.begin()+k);
                T_kk_.Get(i,k,j) = scheme_1st_->Kernel(T.begin()+k);

                mu_k_   .Get(i,k,j) = scheme_1st_->Kernel(mu.begin()+k);
                kappa_k_.Get(i,k,j) = scheme_1st_->Kernel(kappa.begin()+k);
            }
        }
    }

    return 0;
}

int ViscousRun::RunX() {
    if(p_->domain_->ilen_global_==1) {
        return 1;
    }

    for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
        for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> uj      (BUFFER_SIZE, 0.);
            vector<double> uk      (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> vj      (BUFFER_SIZE, 0.);
            vector<double> vk      (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> wj      (BUFFER_SIZE, 0.);
            vector<double> wk      (BUFFER_SIZE, 0.);
            vector<double> T       (BUFFER_SIZE, 0.);
            vector<double> mu      (BUFFER_SIZE, 0.);
            vector<double> kappa   (BUFFER_SIZE, 0.);
 
            for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
                u    [i] = p_->flow_->u_    .GetValue(i,k,j);
                v    [i] = p_->flow_->v_    .GetValue(i,k,j);
                w    [i] = p_->flow_->w_    .GetValue(i,k,j);
                T    [i] = p_->flow_->T_    .GetValue(i,k,j);
                mu   [i] = p_->flow_->mu_   .GetValue(i,k,j);
                kappa[i] = p_->flow_->kappa_.GetValue(i,k,j);
                uj   [i] = u_j_             .GetValue(i,k,j);
                vj   [i] = v_j_             .GetValue(i,k,j);
                wj   [i] = w_j_             .GetValue(i,k,j);
                uk   [i] = u_k_             .GetValue(i,k,j);
                vk   [i] = v_k_             .GetValue(i,k,j);
                wk   [i] = w_k_             .GetValue(i,k,j);
            }
            for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
                u_i_ .Get(i,k,j) = scheme_1st_->Kernel(u.begin()+i);
                u_ii_.Get(i,k,j) = scheme_2nd_->Kernel(u.begin()+i);
                u_ij_.Get(i,k,j) = scheme_1st_->Kernel(uj.begin()+i);
                u_ik_.Get(i,k,j) = scheme_1st_->Kernel(uk.begin()+i);

                v_i_ .Get(i,k,j) = scheme_1st_->Kernel(v.begin()+i);
                v_ii_.Get(i,k,j) = scheme_2nd_->Kernel(v.begin()+i);
                v_ij_.Get(i,k,j) = scheme_1st_->Kernel(vj.begin()+i);

                w_i_ .Get(i,k,j) = scheme_1st_->Kernel(w.begin()+i);
                w_ii_.Get(i,k,j) = scheme_2nd_->Kernel(w.begin()+i);
                w_ik_.Get(i,k,j) = scheme_1st_->Kernel(wk.begin()+i);

                T_i_ .Get(i,k,j) = scheme_1st_->Kernel(T.begin()+i);
                T_ii_.Get(i,k,j) = scheme_2nd_->Kernel(T.begin()+i);

                mu_i_   .Get(i,k,j) = scheme_1st_->Kernel(mu.begin()+i);
                kappa_i_.Get(i,k,j) = scheme_1st_->Kernel(kappa.begin()+i);
            }
        }
    }

    return 0;
}


int ViscousRun::RunV() {
    for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
        for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
            for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
                double didx = p_->mesh_->di_dx_.GetValue(i,k,j);
                double djdy = p_->mesh_->dj_dy_.GetValue(i,k,j);
                double dkdz = p_->mesh_->dk_dz_.GetValue(i,k,j);
                double didxsq = p_->mesh_->di_dx_sq_.GetValue(i,k,j);
                double djdysq = p_->mesh_->dj_dy_sq_.GetValue(i,k,j);
                double dkdzsq = p_->mesh_->dk_dz_sq_.GetValue(i,k,j);
                double ddidx2 = p_->mesh_->ddi_dx2_.GetValue(i,k,j);
                double ddjdy2 = p_->mesh_->ddj_dy2_.GetValue(i,k,j);
                double ddkdz2 = p_->mesh_->ddk_dz2_.GetValue(i,k,j);
 
                double mu    = p_->flow_->mu_   .GetValue(i,k,j);
                double u     = p_->flow_->u_    .GetValue(i,k,j);
                double v     = p_->flow_->v_    .GetValue(i,k,j);
                double w     = p_->flow_->w_    .GetValue(i,k,j);
                double kappa = p_->flow_->kappa_.GetValue(i,k,j);
                double u_x  = u_i_ .GetValue(i,k,j) * didx;
                double u_y  = u_j_ .GetValue(i,k,j) * djdy;
                double u_z  = u_k_ .GetValue(i,k,j) * dkdz;
                double u_xy = u_ij_.GetValue(i,k,j) * didx*djdy;
                double u_xz = u_ik_.GetValue(i,k,j) * didx*dkdz;
                double v_x  = v_i_ .GetValue(i,k,j) * didx;
                double v_y  = v_j_ .GetValue(i,k,j) * djdy;
                double v_z  = v_k_ .GetValue(i,k,j) * dkdz;
                double v_xy = v_ij_.GetValue(i,k,j) * didx*djdy;
                double v_yz = v_jk_.GetValue(i,k,j) * djdy*dkdz;
                double w_x  = w_i_ .GetValue(i,k,j) * didx;
                double w_y  = w_j_ .GetValue(i,k,j) * djdy;
                double w_z  = w_k_ .GetValue(i,k,j) * dkdz;
                double w_xz = w_ik_.GetValue(i,k,j) * didx*dkdz;
                double w_yz = w_jk_.GetValue(i,k,j) * djdy*dkdz;
                double T_x = T_i_.GetValue(i,k,j) * didx;
                double T_y = T_j_.GetValue(i,k,j) * djdy;
                double T_z = T_k_.GetValue(i,k,j) * dkdz;
                double mu_x = mu_i_.GetValue(i,k,j) * didx;
                double mu_y = mu_j_.GetValue(i,k,j) * djdy;
                double mu_z = mu_k_.GetValue(i,k,j) * dkdz;
                double kappa_x = kappa_i_.GetValue(i,k,j) * didx;
                double kappa_y = kappa_j_.GetValue(i,k,j) * djdy;
                double kappa_z = kappa_k_.GetValue(i,k,j) * dkdz;
                double u_xx = u_i_.GetValue(i,k,j) * ddidx2 + u_ii_.GetValue(i,k,j) * didxsq;
                double v_xx = v_i_.GetValue(i,k,j) * ddidx2 + v_ii_.GetValue(i,k,j) * didxsq;
                double w_xx = w_i_.GetValue(i,k,j) * ddidx2 + w_ii_.GetValue(i,k,j) * didxsq;
                double T_xx = T_i_.GetValue(i,k,j) * ddidx2 + T_ii_.GetValue(i,k,j) * didxsq;
                double u_yy = u_j_.GetValue(i,k,j) * ddjdy2 + u_jj_.GetValue(i,k,j) * djdysq;
                double v_yy = v_j_.GetValue(i,k,j) * ddjdy2 + v_jj_.GetValue(i,k,j) * djdysq;
                double w_yy = w_j_.GetValue(i,k,j) * ddjdy2 + w_jj_.GetValue(i,k,j) * djdysq;
                double T_yy = T_j_.GetValue(i,k,j) * ddjdy2 + T_jj_.GetValue(i,k,j) * djdysq;
                double u_zz = u_k_.GetValue(i,k,j) * ddkdz2 + u_kk_.GetValue(i,k,j) * dkdzsq;
                double v_zz = v_k_.GetValue(i,k,j) * ddkdz2 + v_kk_.GetValue(i,k,j) * dkdzsq;
                double w_zz = w_k_.GetValue(i,k,j) * ddkdz2 + w_kk_.GetValue(i,k,j) * dkdzsq;
                double T_zz = T_k_.GetValue(i,k,j) * ddkdz2 + T_kk_.GetValue(i,k,j) * dkdzsq;

                double tauxx_over_mu = 4./3.*u_x - 2./3.*v_y - 2./3.*w_z;
                double tauxy_over_mu =       u_y +       v_x;
                double tauxz_over_mu =       u_z +       w_x;
                double tauyy_over_mu = 4./3.*v_y - 2./3.*u_x - 2./3.*w_z;
                double tauyz_over_mu =       v_z +       w_y;
                double tauzz_over_mu = 4./3.*w_z - 2./3.*u_x - 2./3.*v_y;

                double s_v2 = mu * (4./3.*u_xx + 1./3.*v_xy + 1./3.*w_xz + u_yy + u_zz) + mu_x*tauxx_over_mu + mu_y*tauxy_over_mu + mu_z*tauxz_over_mu;
                double s_v3 = mu * (4./3.*v_yy + 1./3.*u_xy + 1./3.*w_yz + v_xx + v_zz) + mu_x*tauxy_over_mu + mu_y*tauyy_over_mu + mu_z*tauyz_over_mu;
                double s_v4 = mu * (4./3.*w_zz + 1./3.*u_xz + 1./3.*v_yz + w_xx + w_yy) + mu_x*tauxz_over_mu + mu_y*tauyz_over_mu + mu_z*tauzz_over_mu;
                double Q_v = kappa*(T_xx+T_yy+T_zz) + kappa_x*T_x + kappa_y*T_y + kappa_z*T_z;
                double s_v5 = u*s_v2 + v*s_v3 + w*s_v4
                            + mu * (u_x*tauxx_over_mu + v_x*tauxy_over_mu + w_x*tauxz_over_mu + u_y*tauxy_over_mu + v_y*tauyy_over_mu + w_y*tauyz_over_mu 
                                   +u_z*tauxz_over_mu + v_z*tauyz_over_mu + w_z*tauzz_over_mu)
                            + Q_v;

                p_->rhs_->dQ_.Get(i,k,j,1) += s_v2;
                p_->rhs_->dQ_.Get(i,k,j,2) += s_v3;
                p_->rhs_->dQ_.Get(i,k,j,3) += s_v4;
                p_->rhs_->dQ_.Get(i,k,j,4) += s_v5;
               
            }
        }
    }

    return 0;
}

