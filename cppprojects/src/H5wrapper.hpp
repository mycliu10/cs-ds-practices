#pragma once
#include <iostream>
#include <algorithm>
#include "hdf5.h"
#include "array.hpp"

using std::string;

class H5Wrapper {
private:
    string file_name_; 
    string dataset_names_[256];
    int num_datasets_;
    string scalar_names_[256];
    int num_scalars_;
    int dataset_size_;
    Array<double> * buffer_;
    int rank_;
    hsize_t dims_[4];
public:
    H5Wrapper(string & filename, const string *const dataset_names, 
              const int num_datasets, const int *const dims, const int rank,
              Array<double> *const buffer);
    int Read();
    ///
    /// Create file/dataset
    /// In: flag: 0 to truncate new file/dataset, otherwise truncate new dataset
    ///
    int Create(const int flag);

    int Write();

};
