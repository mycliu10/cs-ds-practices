#include "H5wrapper.hpp"

H5Wrapper::H5Wrapper(string & filename, const string *const dataset_names, 
                     const int num_datasets, const int *const dims, const int rank,
                     Array<double> *const buffer) {
    num_datasets_ = num_datasets;
    rank_ = rank;
    file_name_ = filename;
    std::copy(dataset_names, dataset_names+num_datasets_, dataset_names_);
    std::copy(dims, dims+rank_, dims_);
    buffer_ = buffer;

    // check buffer size
    dataset_size_ = 1;
    for(int i=0; i<rank_; ++i) {
        dataset_size_ *= dims_[i];
    }
    int required_size = dataset_size_ * num_datasets_;
    if(required_size > buffer_->size_) {
        std::cout << "Buffer size is insufficient for file " << file_name_ << std::endl;
        exit(-1);
    }

}

int H5Wrapper::Read() {
    hid_t file = H5Fopen(file_name_.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    for(int n=0; n<num_datasets_; ++n) {
        hid_t dset = H5Dopen(file, dataset_names_[n].c_str(), H5P_DEFAULT);
        hid_t dspace = H5Dget_space(dset);
        hsize_t dims[4];
        hsize_t maxsims[4];
        int rank = H5Sget_simple_extent_dims(dspace, dims, maxsims);

        // check size
        int size_check = 0;
        for(int i=0; i<rank_; ++i) {
            if(dims[i] == dims_[i]) {
                size_check += 1;
            }
        }
        if(size_check<rank_) {
            std::cout << "Wrong dimensions in " << dataset_names_[n] 
                      << " of rank " << rank
                      << " in file " << file_name_ << std::endl;
            exit(-1);
        }

        // read data
        herr_t err = H5Dread(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(buffer_->Get(n*dataset_size_)));
        err = H5Dclose(dset);
    } 
    herr_t err = H5Fclose(file);
    
    return err;
}

int H5Wrapper::Create(const int flag) {
    hid_t file;
    if(flag==0) {
        file = H5Fcreate(file_name_.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    }
    else {
        file = H5Fopen(file_name_.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    }
    hid_t dspace = H5Screate_simple(rank_, dims_, NULL);

    for(int n=0; n<num_datasets_; ++n) {
        hid_t dset = H5Dcreate(file, dataset_names_[n].c_str(), H5T_NATIVE_DOUBLE, dspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        herr_t err = H5Dclose(dset);
    }

    herr_t err = H5Sclose(dspace);
    err = H5Fclose(file);

    return 0;
}


int H5Wrapper::Write() {
    hid_t file = H5Fopen(file_name_.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    for(int n=0; n<num_datasets_; ++n) {
        hid_t dset = H5Dopen(file, dataset_names_[n].c_str(), H5P_DEFAULT);

        // write data
        herr_t err = H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(buffer_->Get(n*dataset_size_)));
        err = H5Dclose(dset);
    } 
    herr_t err = H5Fclose(file);
    
    return err;
}


