#include "flow.hpp"

using std::cout;
using std::endl;

Flow::Flow(IdealGas *const gas, Domain *const domain) {
    time_ = 0.;
    gas_ = gas;
    domain_ = domain;
    int ilen = domain_->ihaloend_+1;
    int jlen = domain_->jhaloend_+1;
    int klen = domain_->khaloend_+1;
    u_.     Initialize(ilen, klen, jlen); 
    v_.     Initialize(ilen, klen, jlen); 
    w_.     Initialize(ilen, klen, jlen); 
    p_.     Initialize(ilen, klen, jlen); 
    T_.     Initialize(ilen, klen, jlen); 
    et_.    Initialize(ilen, klen, jlen); 
    a_.     Initialize(ilen, klen, jlen); 
    mu_.    Initialize(ilen, klen, jlen); 
    kappa_. Initialize(ilen, klen, jlen); 
    rho_.   Initialize(ilen, klen, jlen); 
    rhou_.  Initialize(ilen, klen, jlen); 
    rhov_.  Initialize(ilen, klen, jlen); 
    rhow_.  Initialize(ilen, klen, jlen); 
    rhoet_. Initialize(ilen, klen, jlen); 
    buffer_.Initialize(8, domain_->jlen_, domain_->ilen_, domain_->klen_);
    cout << "Flow of size " << domain_->ilen_global_ << " X " << domain_->jlen_global_ << " X " << domain_->klen_global_<< " created." << endl;
    std::cout << "========================================================" << std::endl;
}



int Flow::ReadFlow(const char *filename) {
    string flnm = filename;
    string dnames[5] = {"u", "v", "w", "p", "T"};
    int dimensions[3] = {domain_->jlen_global_, domain_->ilen_global_, domain_->klen_global_};
    H5Wrapper h5flow(flnm, dnames, 5, dimensions, 3, &buffer_);
    h5flow.Read();
   
    // assign values 
    for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
        for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
            for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
                int idx = u_.GetIndex(i,k,j);
                u_.Get(idx) = buffer_.GetValue(0,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                v_.Get(idx) = buffer_.GetValue(1,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                w_.Get(idx) = buffer_.GetValue(2,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                p_.Get(idx) = buffer_.GetValue(3,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                T_.Get(idx) = buffer_.GetValue(4,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_);
                UpdateFlowFromPrimitive(idx);
            }
        }
    }

    std::cout << "Flow read succesfully." << std::endl;
    std::cout << "========================================================" << std::endl;
    return 0;
}

int Flow::WriteFlow(const char *filename) {
    buffer_.Zero();
    for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
        for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
            for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
                buffer_.Get(0,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_) = u_.GetValue(i,k,j);
                buffer_.Get(1,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_) = v_.GetValue(i,k,j);
                buffer_.Get(2,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_) = w_.GetValue(i,k,j);
                buffer_.Get(3,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_) = p_.GetValue(i,k,j);
                buffer_.Get(4,j-domain_->jbeg_,i-domain_->ibeg_,k-domain_->kbeg_) = T_.GetValue(i,k,j);
            }
        }
    }
 

    string flnm = filename;
    string dnames[5] = {"u", "v", "w", "p", "T"};
    int dimensions[3] = {domain_->jlen_global_, domain_->ilen_global_, domain_->klen_global_};
    H5Wrapper h5flow(flnm, dnames, 5, dimensions, 3, &buffer_);
    h5flow.Create(0);
    h5flow.Write();
    
    return 0;
}
 




