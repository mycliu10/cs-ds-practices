#include "mesh.hpp"
#include "polynomial.hpp"
#include "central_difference.hpp"

using std::cout;
using std::endl;

Mesh::Mesh(Domain *const domain) {
    domain_ = domain;
    num_points_to_extend_ = domain_->stencil_ + num_extra_points_to_extend_;

    // initialize arrays
    buffer_read_ .Initialize(3,domain_->jlen_, domain_->ilen_, domain_->klen_);
    int ilen = domain_->ihaloend_ + 2*num_extra_points_to_extend_ + 1;
    int jlen = domain_->jhaloend_ + 2*num_extra_points_to_extend_ + 1;
    int klen = domain_->khaloend_ + 2*num_extra_points_to_extend_ + 1;
    buffer_write_.Initialize(6, jlen, ilen, klen);

    buffer_mesh_. Initialize(3, ilen, klen, jlen);
    x_.    Initialize(ilen, klen, jlen);
    y_.    Initialize(ilen, klen, jlen);
    z_.    Initialize(ilen, klen, jlen);
    ilen = domain_->ihaloend_ + 1; 
    jlen = domain_->jhaloend_ + 1;
    klen = domain_->khaloend_ + 1;
    dx_di_.Initialize(ilen, klen, jlen);
    dx_dj_.Initialize(ilen, klen, jlen);
    dx_dk_.Initialize(ilen, klen, jlen);
    dy_di_.Initialize(ilen, klen, jlen);
    dy_dj_.Initialize(ilen, klen, jlen);
    dy_dk_.Initialize(ilen, klen, jlen);
    dz_di_.Initialize(ilen, klen, jlen);
    dz_dj_.Initialize(ilen, klen, jlen);
    dz_dk_.Initialize(ilen, klen, jlen);
    di_dx_.Initialize(ilen, klen, jlen);
    di_dy_.Initialize(ilen, klen, jlen);
    di_dz_.Initialize(ilen, klen, jlen);
    dj_dx_.Initialize(ilen, klen, jlen);
    dj_dy_.Initialize(ilen, klen, jlen);
    dj_dz_.Initialize(ilen, klen, jlen);
    dk_dx_.Initialize(ilen, klen, jlen);
    dk_dy_.Initialize(ilen, klen, jlen);
    dk_dz_.Initialize(ilen, klen, jlen);

    di_dx_sq_.Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);
    dj_dy_sq_.Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);
    dk_dz_sq_.Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);
    ddi_dx2_. Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);
    ddj_dy2_. Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);
    ddk_dz2_. Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1);

    scheme_1st_ = new CDFamily(1,2);
    scheme_2nd_ = new CDFamily(2,2);

    cout << "Mesh of size " << domain_->ilen_global_ << " X " << domain_->jlen_global_ << " X " << domain_->klen_global_ <<" created." << endl;
    std::cout << "========================================================" << std::endl;
}


int Mesh::ReadMesh(const char *filename) {
    string flnm = filename;
    string dnames[3] = {"x", "y", "z"};
    int dimensions[3] = {domain_->jlen_global_, domain_->ilen_global_, domain_->klen_global_};
    H5Wrapper h5mesh(flnm, dnames, 3, dimensions, 3, &buffer_read_);
    h5mesh.Read();
   
    // assign values 
//    int M = num_extra_points_to_extend_;
//    for(int n=0; n<3; ++n) {
//        for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
//            for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
//                for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
//                    buffer_mesh_.Get(n,i+M,k+M,j+M) = buffer_read_.GetValue(n,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
//                }
//            }
//        }
//    }

    for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
        for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
            for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
                x_.Get(i,k,j) = buffer_read_.GetValue(0,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                y_.Get(i,k,j) = buffer_read_.GetValue(1,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                z_.Get(i,k,j) = buffer_read_.GetValue(2,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
            }
        }
    }
 

    std::cout << "Mesh read succesfully." << std::endl;
    std::cout << "========================================================" << std::endl;
    return 0;
}


int Mesh::ExtendMesh() {
    const int order = 2;
    const int N = order + 1;
    int M = num_extra_points_to_extend_;
    double index[N];
    double coord[N*3];
    double coefs[N*3];

    for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
        for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
            for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
                buffer_mesh_.Get(0,i+M,k+M,j+M) = x_.GetValue(i,k,j);
                buffer_mesh_.Get(1,i+M,k+M,j+M) = y_.GetValue(i,k,j);
                buffer_mesh_.Get(2,i+M,k+M,j+M) = z_.GetValue(i,k,j);
            }
        }
    }



    if(domain_->jlen_global_>1) {
        for(int i = domain_->ibeg_; i <= domain_->iend_; ++i) {  
            for(int k = domain_->kbeg_; k <= domain_->kend_; ++k) {  
                for(int j = domain_->jbeg_; j <= domain_->jbeg_+order; ++j) {  
                    int n = j - domain_->jbeg_;
                    index[n] = double(j);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int j = domain_->jhalobeg_-M; j < domain_->jbeg_; ++j) {  
                    Polynomial::GetValues(coefs, double(j), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }
    
                for(int j = domain_->jend_-order; j <= domain_->jend_; ++j) {  
                    int n = j - domain_->jend_+order;
                    index[n] = double(j);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int j = domain_->jend_+1; j <= domain_->jhaloend_+M; ++j) {  
                    Polynomial::GetValues(coefs, double(j), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }
            }
        }
    }
    else {
        for(int n=0; n<3; ++n) {
            for(int i = domain_->ibeg_; i <= domain_->iend_; ++i) {  
                for(int k = domain_->kbeg_; k <= domain_->kend_; ++k) {  
                    for(int j = domain_->jhalobeg_-M; j <= domain_->jhaloend_+M; ++j) {  
                        buffer_mesh_.Get(n,i+M,k+M,j+M) = buffer_mesh_.GetValue(n,i+M,k+M,domain_->jbeg_+M);
                    }
                }
            }
        }
    }

    if(domain_->klen_global_>1) {
        for(int i = domain_->ibeg_; i <= domain_->iend_; ++i) {  
            for(int j = domain_->jhalobeg_-M; j <= domain_->jhaloend_+M; ++j) {  
                for(int k = domain_->kbeg_; k <= domain_->kbeg_+order; ++k) {  
                    int n = k - domain_->kbeg_;
                    index[n] = double(k);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int k = domain_->khalobeg_-M; k < domain_->kbeg_; ++k) {  
                    Polynomial::GetValues(coefs, double(k), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }

                for(int k = domain_->kend_-order; k <= domain_->kend_; ++k) {  
                    int n = k - domain_->kend_+order;
                    index[n] = double(k);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int k = domain_->kend_+1; k <= domain_->khaloend_+M; ++k) {  
                    Polynomial::GetValues(coefs, double(k), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }
            }
        }
    }

    if(domain_->ilen_global_>1) {
        for(int k = domain_->khalobeg_-M; k <= domain_->khaloend_+M; ++k) {  
            for(int j = domain_->jhalobeg_-M; j <= domain_->jhaloend_+M; ++j) {  
                for(int i = domain_->ibeg_; i <= domain_->ibeg_+order; ++i) {  
                    int n = i - domain_->ibeg_;
                    index[n] = double(i);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int i = domain_->ihalobeg_-M; i < domain_->ibeg_; ++i) {  
                    Polynomial::GetValues(coefs, double(i), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }
    
                for(int i = domain_->iend_-order; i <= domain_->iend_; ++i) {  
                    int n = i - domain_->iend_+order;
                    index[n] = double(i);
                    coord[n*N  ] = buffer_mesh_.GetValue(0,i+M,k+M,j+M); 
                    coord[n*N+1] = buffer_mesh_.GetValue(1,i+M,k+M,j+M); 
                    coord[n*N+2] = buffer_mesh_.GetValue(2,i+M,k+M,j+M); 
                }
                Polynomial::GetCoefficients(index, coord, coefs, order, 3);
                for(int i = domain_->iend_+1; i <= domain_->ihaloend_+M; ++i) {  
                    Polynomial::GetValues(coefs, double(i), coord, order, 3);
                    buffer_mesh_.Get(0,i+M,k+M,j+M) = coord[0];
                    buffer_mesh_.Get(1,i+M,k+M,j+M) = coord[1];
                    buffer_mesh_.Get(2,i+M,k+M,j+M) = coord[2];
                }
            }
        }
    }

    // assign values
    for(int j=domain_->jhalobeg_; j<=domain_->jhaloend_; ++j) {
        for(int i=domain_->ihalobeg_; i<=domain_->ihaloend_; ++i) {
            for(int k=domain_->khalobeg_; k<=domain_->khaloend_; ++k) {
                x_.Get(i,k,j) = buffer_mesh_.GetValue(0,i+M,k+M,j+M);
                y_.Get(i,k,j) = buffer_mesh_.GetValue(1,i+M,k+M,j+M);
                z_.Get(i,k,j) = buffer_mesh_.GetValue(2,i+M,k+M,j+M);
            }
        }
    }
 

    // write extended mesh
    int ilen = domain_->ihaloend_ + 2*M+ 1;
    int jlen = domain_->jhaloend_ + 2*M+ 1;
    int klen = domain_->khaloend_ + 2*M+ 1;
    for(int n=0; n<3; ++n) {
        for(int i=0; i<ilen; ++i) {
            for(int k=0; k<klen; ++k) {
                for(int j=0; j<jlen; ++j) {
                    buffer_write_.Get(n,j,i,k) = buffer_mesh_.GetValue(n,i,k,j);
                }
            }
        }
    }
    string flnm = "grid.ext.h5";
    string dnames[3] = {"x", "y", "z"};
    int dimensions[3] = {jlen, ilen, klen};
    H5Wrapper h5mesh(flnm, dnames, 3, dimensions, 3, &buffer_write_);
    h5mesh.Create(0);
    h5mesh.Write();
    
    std::cout << "Mesh extended." << std::endl;
    std::cout << "========================================================" << std::endl;

    return 0;
}


int Mesh::CalculateMetrics() {
    int M = num_extra_points_to_extend_;
    for(int i = domain_->ihalobeg_; i <= domain_->ihaloend_; ++i) {  
        for(int k = domain_->khalobeg_; k <= domain_->khaloend_; ++k) {  
            for(int j = domain_->jhalobeg_; j <= domain_->jhaloend_; ++j) {  
                vector<double> f(3, 0.);
                for(int n=0; n<scheme_1st_->stencil_; ++n) {
                    f[n] = buffer_mesh_.GetValue(0,i+scheme_1st_->offset_+n+M,k+M,j+M);
                }
                double dxdi = scheme_1st_->Kernel(f.begin());
                double d2xdi2 = scheme_2nd_->Kernel(f.begin());
                
                double dydj = 1.;
                double d2ydj2 = 0.;
                if(domain_->jlen_global_>1) {
                    for(int n=0; n<scheme_1st_->stencil_; ++n) {
                        f[n] = buffer_mesh_.GetValue(1,i+M,k+M,j+scheme_1st_->offset_+n+M);
                    }
                    dydj = scheme_1st_->Kernel(f.begin());
                    d2ydj2 = scheme_2nd_->Kernel(f.begin());
                }

                for(int n=0; n<scheme_1st_->stencil_; ++n) {
                    f[n] = buffer_mesh_.GetValue(2,i+M,k+scheme_1st_->offset_+n+M,j+M);
                }
                double dzdk = scheme_1st_->Kernel(f.begin());
                double d2zdk2 = scheme_2nd_->Kernel(f.begin());


                dx_di_.Get(i,k,j) = dxdi;
                dy_dj_.Get(i,k,j) = dydj;
                dz_dk_.Get(i,k,j) = dzdk;

                if(domain_->ilen_global_>1) {
                    di_dx_.Get(i,k,j) = 1. / dxdi;
                }
                else {
                    di_dx_.Get(i,k,j) = 0.;
                }
                if(domain_->jlen_global_>1) {
                    dj_dy_.Get(i,k,j) = 1. / dydj;
                }
                else {
                    dj_dy_.Get(i,k,j) = 0.;
                }
                if(domain_->klen_global_>1) {
                    dk_dz_.Get(i,k,j) = 1. / dzdk;
                }
                else {
                    dk_dz_.Get(i,k,j) = 0.;
                }

                di_dx_sq_.Get(i,k,j) = std::pow(di_dx_.GetValue(i,k,j), 2.);
                dj_dy_sq_.Get(i,k,j) = std::pow(dj_dy_.GetValue(i,k,j), 2.);
                dk_dz_sq_.Get(i,k,j) = std::pow(dk_dz_.GetValue(i,k,j), 2.);

                ddi_dx2_.Get(i,k,j) = - d2xdi2 / std::pow(dxdi,3.);
                ddj_dy2_.Get(i,k,j) = - d2ydj2 / std::pow(dydj,3.);
                ddk_dz2_.Get(i,k,j) = - d2zdk2 / std::pow(dzdk,3.);
            }
        }
    }

    // write metrics
    buffer_write_.Zero();
    int ilen = domain_->ihaloend_ + 2*M+ 1;
    int jlen = domain_->jhaloend_ + 2*M+ 1;
    int klen = domain_->khaloend_ + 2*M+ 1;
    for(int j=domain_->jhalobeg_; j<=domain_->jhaloend_; ++j) {
        for(int i=domain_->ihalobeg_; i<=domain_->ihaloend_; ++i) {
            for(int k=domain_->khalobeg_; k<=domain_->khaloend_; ++k) {
                buffer_write_.Get(0,j+M,i+M,k+M) = di_dx_  .GetValue(i,k,j);
                buffer_write_.Get(1,j+M,i+M,k+M) = dj_dy_  .GetValue(i,k,j);
                buffer_write_.Get(2,j+M,i+M,k+M) = dk_dz_  .GetValue(i,k,j);
                buffer_write_.Get(3,j+M,i+M,k+M) = ddi_dx2_.GetValue(i,k,j);
                buffer_write_.Get(4,j+M,i+M,k+M) = ddj_dy2_.GetValue(i,k,j);
                buffer_write_.Get(5,j+M,i+M,k+M) = ddk_dz2_.GetValue(i,k,j);
            }
        }
    }
    string flnm = "grid.ext.h5";
    string dnames[6] = {"didx", "djdy", "dkdz", "ddidx2", "ddjdy2", "ddkdz2"};
    int dimensions[3] = {jlen, ilen, klen};
    H5Wrapper h5mesh(flnm, dnames, 6, dimensions, 3, &buffer_write_);
    h5mesh.Create(1);
    h5mesh.Write();


    return 0;
}

