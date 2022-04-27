#include "RHS.hpp"
using std::cout;
using std::endl;

int RHS::Zero() {
    dQ_.Zero();
    dQ0_.Zero();

    return 0;
}    

int RHS::Multiply(const double f) {
    for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
        for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
            for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
	            for(int n=0; n<5; ++n) {
	                dQ_.Get(i,k,j,n) *= f;
                }
	        }
        }
    }	
    return 0;

}



RHS::RHS(Domain *const domain) {
    domain_ = domain;
    dQ_. Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1, 5); 
    dQ0_.Initialize(domain_->ihaloend_+1, domain_->khaloend_+1, domain_->jhaloend_+1, 5); 
    cout << "RHS  of size " << domain_->ilen_global_ << " X " << domain_->jlen_global_ << " X " << domain_->klen_global_<< " created." << endl;
} 


double RHS::GetNorm() {
    double norm = 0.;
    for(int i=domain_->ibeg_; i<=domain_->iend_; ++i) {
        for(int k=domain_->kbeg_; k<=domain_->kend_; ++k) {
            for(int j=domain_->jbeg_; j<=domain_->jend_; ++j) {
                double l = std::abs(dQ_.GetValue(i,k,j,0));
                if(l > norm) {
                    norm = l;
                }
            }
        }
    }

    return norm;
}
