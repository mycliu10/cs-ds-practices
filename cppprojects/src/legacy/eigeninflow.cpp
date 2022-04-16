#include "eigeninflow.hpp"
#include "domain.hpp"
#include "hdf5.h"

#include <iostream>

using namespace std;
const complex<double> imaginary_unit(0, 1);

int EigenInflow::Initialize(int num_mode) {
    num_mode_ = num_mode;
    omega_ = new complex<double>[num_mode_];
    uhat_.Initialize(domain::ibeg, domain::jlen);
    vhat_.Initialize(domain::ibeg, domain::jlen);
    rhohat_.Initialize(domain::ibeg, domain::jlen);
    phat_.Initialize(domain::ibeg, domain::jlen);
    That_.Initialize(domain::ibeg, domain::jlen);
    cout << "Initialized eigeninflow." << endl;
}


int EigenInflow::ReadEigen() {
    double ** buff;
    int err = NativeArray2D(buff, domain::ibeg, domain::jnpt);
    
    hid_t file_id = H5Fopen("eigenfunc_0001.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    
    // omega
    double * omega = new double[1];
    hid_t dset_id = H5Dopen(file_id, "omr", H5P_DEFAULT);
    herr_t herr = H5Dread(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, omega);
    omega_[0] = omega[0];    
    herr = H5Dclose(dset_id);

    // dist
    ReadDistDataset(file_id, "ur", "ui", uhat_, buff);
    ReadDistDataset(file_id, "vr", "vi", uhat_, buff);
    ReadDistDataset(file_id, "pr", "pi", uhat_, buff);
    ReadDistDataset(file_id, "tr", "ti", uhat_, buff);
    
    cout << "Read eigeninflow." << endl;

}


int EigenInflow::ReadDistDataset(hid_t file_id, char * filename_real, char * filename_imag, Array2D<std::complex<double>> fhat, double **buff ){
    hid_t dset_id = H5Dopen(file_id, filename_real, H5P_DEFAULT);
    herr_t herr = H5Dread(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buff);
    for(int i=0; i<domain::ibeg; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            fhat.Get(i,j+domain::jbeg) = buff[i][j];
         }
    }
    herr = H5Dclose(dset_id);
    
    dset_id = H5Dopen(file_id, filename_imag, H5P_DEFAULT);
    herr = H5Dread(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buff);
    for(int i=0; i<domain::ibeg; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            fhat.Get(i,j+domain::jbeg) += imaginary_unit*buff[i][j];
         }
    }
    herr = H5Dclose(dset_id);
    
    return 0;
}



int EigenInflow::ForceEigenInflow(FlowField & flow, const double time) {
    complex<double> time_coef = exp(-imaginary_unit*omega_[0]*time);
//    cout << time_coef << endl;    
    for(int i=0; i<domain::ibeg; i++) {
        for(int j=domain::jbeg; j<domain::jend; j++) {
            complex<double> dist_coef = time_coef*.01;//uhat_.Get(i,j);
            flow.vars.Get(i,j).u_ = flow.baseflow_.u_.Get(i,j) + dist_coef.real();
//            dist_coef = time_coef*vhat_.Get(i,j);
//            flow.vars.Get(i,j).v_ = flow.baseflow_.v_.Get(i,j) + dist_coef.real();
//            dist_coef = time_coef*phat_.Get(i,j);
//            flow.vars.Get(i,j).p_ = flow.baseflow_.p_.Get(i,j) + dist_coef.real();
//            dist_coef = time_coef*That_.Get(i,j);
//            flow.vars.Get(i,j).T_ = flow.baseflow_.T_.Get(i,j) + dist_coef.real();
            flow.vars.Get(i,j).Get_everything_from_primitive();
//            cout << flow.vars.Get(i,j).u_ << endl;    
        }
    }
}
            

