#pragma once
#include "array.hpp"
#include "hdf5.h"
#include "everything.hpp"
#include "baseflow.hpp"
#include <complex>

class EigenInflow {
private:
    int num_mode_;
    std::complex<double> * omega_;
    Array2D<std::complex<double>> uhat_, vhat_, rhohat_, phat_, That_;
    
    int ReadDistDataset(hid_t file_id, char * filename_real, char * filename_imag, Array2D<std::complex<double>> fhat, double **buff);

public:
    int Initialize(int num_mode);
    int ReadEigen();
    int ForceEigenInflow(FlowField & flow, const double time);



};


