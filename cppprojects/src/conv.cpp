#include "conv.hpp"

const static int BUFFER_SIZE = 1024;


template <class It>
int ConvectionRun::RunLine(const int direction, 
                           It iter_uu, It iter_fu,
                           It iter_p,  It iter_a,
                           It iter_u,  It iter_v, It iter_w,
                           It iter_ustar, It iter_ustarma, It iter_ustarpa, 
                           It iter_fhat) {
    double uhat = 0.;
    double vhat = 0.;
    double what = 0.;
    double ahat = 0.;
    if(do_reconstruction_==1) {
        double   rhol = *(iter_uu-5);
        double   rhor = *(iter_uu);
        double rhoetl = *(iter_uu-5+4);
        double rhoetr = *(iter_uu+4);
        double     pl = *(iter_p -1);
        double     pr = *(iter_p);
        double     ul = *(iter_u -1);
        double     ur = *(iter_u);
        double     vl = *(iter_v -1);
        double     vr = *(iter_v);
        double     wl = *(iter_w -1);
        double     wr = *(iter_w);
    
        averager_->Kernel(rhol, rhor, ul, ur, vl, vr, wl, wr, pl, pr, rhoetl, rhoetr,
                          uhat, vhat, what, ahat);
    }
    else {
        uhat = *(iter_u);
        vhat = *(iter_v);
        what = *(iter_w);
        ahat = *(iter_a);
    }

    vector<double> cc(8*5, 0.);
    vector<double> fc(8*5, 0.);
    if(do_characteristic_==1) {
        for(int n=0; n<scheme_->stencil_; ++n) {
            vector<double> c(5, 0.);
            transformer_->PhysicalToCharacteristicCartesian(uhat, vhat, what, ahat, direction, 
															iter_uu+(scheme_->offset_+n)*5, 
												            c.begin());
            for(int m=0; m<5; ++m) {
                cc[m*scheme_->stencil_+n] = c[m];
            }
            transformer_->PhysicalToCharacteristicCartesian(uhat, vhat, what, ahat, direction, 
															iter_fu+(scheme_->offset_+n)*5, 
												            c.begin());
            for(int m=0; m<5; ++m) {
                fc[m*scheme_->stencil_+n] = c[m];
            }
        }
    }
    else {
        for(int n=0; n<scheme_->stencil_; ++n) {
            for(int m=0; m<5; ++m) {
                fc[m*scheme_->stencil_+n] = *(iter_fu+5*(scheme_->offset_+n)+m);
            }
        }
    }

    // split if necessary
    vector<double> fp(8*5, 0.);
    vector<double> fm(8*5, 0.);
    if(do_split_==1) {
        splitter_->Split(iter_ustar  +scheme_->offset_, 
                         iter_ustarpa+scheme_->offset_,
                         iter_ustarma+scheme_->offset_,
                         cc.begin(), fc.begin(),
                         fp.begin(), fm.begin(), scheme_->stencil_);
    }
    else {
        for(int m=0; m<5; ++m) {
            for(int n=0; n<scheme_->stencil_; ++n) {
                fp[m*scheme_->stencil_+n] = fc[m*scheme_->stencil_+n];
            }
        }
    }

    // apply scheme
    vector<double> fchat(5, 0.);    
    for(int n=0; n<5; ++n) {
        double dfp = scheme_->Kernel(fp.begin()+n*scheme_->stencil_-scheme_->offset_); 
        if(do_split_==1) {
            double dfm = scheme_->Kernel(fm.begin()+n*scheme_->stencil_-scheme_->offset_); 
            if(do_reconstruction_==1) {
                splitter_->Merge(dfp, dfm, fchat[n]);
            }
            else {
                splitter_->MergeDerivative(dfp, dfm, fchat[n]);
            }
        }
        else {
            fchat[n] = dfp;
        }
    }

    // back to physical domain if necessary
    if(do_characteristic_==1) {
        transformer_->CharacteristicToPhysicalCartesian(uhat, vhat, what, ahat, direction,
                                                        fchat.begin(), iter_fhat);
    }
    else {
        for(int n=0; n<5; ++n) {
            *(iter_fhat+n) = fchat[n];
        }
    }

    return 0;
}


           

int ConvectionRun::RunX() {
    if(p_->domain_->ilen_global_==1) {
        return 1;
    }

    global_profiler.Start(1);

#pragma omp parallel for collapse(2) 
    for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
        for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
	        // Follow i line
	        // prepare flux
            vector<double> p       (BUFFER_SIZE, 0.);
            vector<double> a       (BUFFER_SIZE, 0.);
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> ustar   (BUFFER_SIZE, 0.);
            vector<double> ustarma (BUFFER_SIZE, 0.);
            vector<double> ustarpa (BUFFER_SIZE, 0.);
            vector<double> uu    (BUFFER_SIZE*5, 0.);
            vector<double> fu    (BUFFER_SIZE*5, 0.);
            vector<double> fhat  (BUFFER_SIZE*5, 0.);
            for(int i=p_->domain_->ihalobeg_; i<=p_->domain_->ihaloend_; ++i) {
                int idx = p_->flow_->rho_.GetIndex(i,k,j);
                p[i]       =  p_->flow_->    p_.GetValue(idx);
                a[i]       =  p_->flow_->    a_.GetValue(idx);
                u[i]       =  p_->flow_->    u_.GetValue(idx);
                v[i]       =  p_->flow_->    v_.GetValue(idx);
                w[i]       =  p_->flow_->    w_.GetValue(idx);
	            uu[5*i  ]  =  p_->flow_->  rho_.GetValue(idx);
                uu[5*i+1]  =  p_->flow_-> rhou_.GetValue(idx);
                uu[5*i+2]  =  p_->flow_-> rhov_.GetValue(idx);
                uu[5*i+3]  =  p_->flow_-> rhow_.GetValue(idx);
                uu[5*i+4]  =  p_->flow_->rhoet_.GetValue(idx);
                fu[5*i  ]  =  p_->flow_-> rhou_.GetValue(idx);
                fu[5*i+1]  =  p_->flow_-> rhou_.GetValue(idx) * p_->flow_->u_.GetValue(idx)  + p_->flow_->p_.GetValue(idx); 
                fu[5*i+2]  =  p_->flow_-> rhov_.GetValue(idx) * p_->flow_->u_.GetValue(idx);
                fu[5*i+3]  =  p_->flow_-> rhow_.GetValue(idx) * p_->flow_->u_.GetValue(idx);
                fu[5*i+4]  = (p_->flow_->rhoet_.GetValue(idx) + p_->flow_->p_.GetValue(idx)) * p_->flow_->u_.GetValue(idx);
                ustar[i]   =  std::abs(p_->flow_->    u_.GetValue(idx));
	            ustarma[i] =  std::abs(p_->flow_->    u_.GetValue(idx) - p_->flow_->a_.GetValue(idx));
	            ustarpa[i] =  std::abs(p_->flow_->    u_.GetValue(idx) + p_->flow_->a_.GetValue(idx));
            }

           	//  apply weno
            //  Points:       ibeg,         ibeg+1, ..., iend-1,       iend
            //  Flux:   ibeg,       ibeg+1, ...,                 iend,      iend+1
            for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_+1; ++i) {
                RunLine(0, 
                        uu.begin()+i, fu.begin()+i, 
                        p.begin()+i, a.begin()+i, 
                        u.begin()+i, v.begin()+i, w.begin()+i, 
                        ustar.begin()+i, ustarma.begin()+i, ustarpa.begin()+i, 
                        fhat.begin()+i);
            }

        	// update RHS
            if(do_reconstruction_==1) {
                for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*(i+1)+n] - fhat[5*i+n];
                        dq *= p_->mesh_->di_dx_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
            else {
                for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*i+n];
                        dq *= p_->mesh_->di_dx_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
                
 
        }
    }

    global_profiler.Pause(1);

    return 0;
}


int ConvectionRun::RunY() {
    if(p_->domain_->jlen_global_==1) {
        return 1;
    }

    global_profiler.Start(2);

#pragma omp parallel for collapse(2) 
    for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
        for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
	        // Follow i line
	        // prepare flux
            vector<double> p       (BUFFER_SIZE, 0.);
            vector<double> a       (BUFFER_SIZE, 0.);
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> ustar   (BUFFER_SIZE, 0.);
            vector<double> ustarma (BUFFER_SIZE, 0.);
            vector<double> ustarpa (BUFFER_SIZE, 0.);
            vector<double> uu    (BUFFER_SIZE*5, 0.);
            vector<double> fu    (BUFFER_SIZE*5, 0.);
            vector<double> fhat  (BUFFER_SIZE*5, 0.);
            for(int j=p_->domain_->jhalobeg_; j<=p_->domain_->jhaloend_; ++j) {
                int idx = p_->flow_->rho_.GetIndex(i,k,j);
                p[j]       =  p_->flow_->    p_.GetValue(idx);
                a[j]       =  p_->flow_->    a_.GetValue(idx);
                u[j]       =  p_->flow_->    u_.GetValue(idx);
                v[j]       =  p_->flow_->    v_.GetValue(idx);
                w[j]       =  p_->flow_->    w_.GetValue(idx);
	            uu[5*j  ]  =  p_->flow_->  rho_.GetValue(idx);
                uu[5*j+1]  =  p_->flow_-> rhou_.GetValue(idx);
                uu[5*j+2]  =  p_->flow_-> rhov_.GetValue(idx);
                uu[5*j+3]  =  p_->flow_-> rhow_.GetValue(idx);
                uu[5*j+4]  =  p_->flow_->rhoet_.GetValue(idx);
                fu[5*j  ]  =  p_->flow_-> rhov_.GetValue(idx);
                fu[5*j+1]  =  p_->flow_-> rhou_.GetValue(idx) * p_->flow_->v_.GetValue(idx); 
                fu[5*j+2]  =  p_->flow_-> rhov_.GetValue(idx) * p_->flow_->v_.GetValue(idx)  + p_->flow_->p_.GetValue(idx);
                fu[5*j+3]  =  p_->flow_-> rhow_.GetValue(idx) * p_->flow_->v_.GetValue(idx);
                fu[5*j+4]  = (p_->flow_->rhoet_.GetValue(idx) + p_->flow_->p_.GetValue(idx)) * p_->flow_->v_.GetValue(idx);
                ustar[j]   =  std::abs(p_->flow_->    v_.GetValue(idx));
	            ustarma[j] =  std::abs(p_->flow_->    v_.GetValue(idx) - p_->flow_->a_.GetValue(idx));
	            ustarpa[j] =  std::abs(p_->flow_->    v_.GetValue(idx) + p_->flow_->a_.GetValue(idx));
            }
           	// apply weno
                for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_+1; ++j) {
                    RunLine(1, 
                            uu.begin()+j, fu.begin()+j, 
                            p.begin()+j, a.begin()+j, 
                            u.begin()+j, v.begin()+j, w.begin()+j,
                            ustar.begin()+j, ustarma.begin()+j, ustarpa.begin()+j, 
                            fhat.begin()+j);
 
                }
            if(do_reconstruction_==1) {
        	// update RHS
                for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*(j+1)+n] - fhat[5*j+n];
                        dq *= p_->mesh_->dj_dy_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
            else {
                for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*j+n];
                        dq *= p_->mesh_->dj_dy_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
 
        }
    }

    global_profiler.Pause(2);
    return 0;
}


int ConvectionRun::RunZ() {
    if(p_->domain_->klen_global_==1) {
        return 1;
    }
 
    global_profiler.Start(3);
#pragma omp parallel for collapse(2) 
    for(int i=p_->domain_->ibeg_; i<=p_->domain_->iend_; ++i) {
        for(int j=p_->domain_->jbeg_; j<=p_->domain_->jend_; ++j) {
	        // Follow i line
	        // prepare flux
            vector<double> p       (BUFFER_SIZE, 0.);
            vector<double> a       (BUFFER_SIZE, 0.);
            vector<double> u       (BUFFER_SIZE, 0.);
            vector<double> v       (BUFFER_SIZE, 0.);
            vector<double> w       (BUFFER_SIZE, 0.);
            vector<double> ustar   (BUFFER_SIZE, 0.);
            vector<double> ustarma (BUFFER_SIZE, 0.);
            vector<double> ustarpa (BUFFER_SIZE, 0.);
            vector<double> uu    (BUFFER_SIZE*5, 0.);
            vector<double> fu    (BUFFER_SIZE*5, 0.);
            vector<double> fhat  (BUFFER_SIZE*5, 0.);
            for(int k=p_->domain_->khalobeg_; k<=p_->domain_->khaloend_; ++k) {
                int idx = p_->flow_->rho_.GetIndex(i,k,j);
                p[k]       =  p_->flow_->    p_.GetValue(idx);
                a[k]       =  p_->flow_->    a_.GetValue(idx);
                u[k]       =  p_->flow_->    u_.GetValue(idx);
                v[k]       =  p_->flow_->    v_.GetValue(idx);
                w[k]       =  p_->flow_->    w_.GetValue(idx);
	            uu[5*k  ]  =  p_->flow_->  rho_.GetValue(idx);
                uu[5*k+1]  =  p_->flow_-> rhou_.GetValue(idx);
                uu[5*k+2]  =  p_->flow_-> rhov_.GetValue(idx);
                uu[5*k+3]  =  p_->flow_-> rhow_.GetValue(idx);
                uu[5*k+4]  =  p_->flow_->rhoet_.GetValue(idx);
                fu[5*k  ]  =  p_->flow_-> rhow_.GetValue(idx);
                fu[5*k+1]  =  p_->flow_-> rhou_.GetValue(idx) * p_->flow_->w_.GetValue(idx); 
                fu[5*k+2]  =  p_->flow_-> rhov_.GetValue(idx) * p_->flow_->w_.GetValue(idx);
                fu[5*k+3]  =  p_->flow_-> rhow_.GetValue(idx) * p_->flow_->w_.GetValue(idx)  + p_->flow_->p_.GetValue(idx);
                fu[5*k+4]  = (p_->flow_->rhoet_.GetValue(idx) + p_->flow_->p_.GetValue(idx)) * p_->flow_->w_.GetValue(idx);
                ustar[k]   =  std::abs(p_->flow_->    w_.GetValue(idx));
	            ustarma[k] =  std::abs(p_->flow_->    w_.GetValue(idx) - p_->flow_->a_.GetValue(idx));
	            ustarpa[k] =  std::abs(p_->flow_->    w_.GetValue(idx) + p_->flow_->a_.GetValue(idx));
            }
           	// apply weno
                for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_+1; ++k) {
                    RunLine(2, 
                            uu.begin()+k, fu.begin()+k, 
                            p.begin()+k, a.begin()+k, 
                            u.begin()+k, v.begin()+k, w.begin()+k, 
                            ustar.begin()+k, ustarma.begin()+k, ustarpa.begin()+k, 
                            fhat.begin()+k);
 
                }
            if(do_reconstruction_==1) {
        	// update RHS
                for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*(k+1)+n] - fhat[5*k+n];
                        dq *= p_->mesh_->dk_dz_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
            else {
                for(int k=p_->domain_->kbeg_; k<=p_->domain_->kend_; ++k) {
                    for(int n=0; n<5; ++n) {
                        double dq = fhat[5*k+n];
                        dq *= p_->mesh_->dk_dz_.GetValue(i,k,j);
                        p_->rhs_->dQ_.Get(i,k,j,n) -= dq;
                    }
                }
            }
 
        }
    }

    global_profiler.Pause(3);
    return 0;
}

