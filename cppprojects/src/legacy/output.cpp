#include "output.hpp"
#include "everything.hpp"
#include <string>
#include <sstream>
#include <iomanip>


double ** output::buf; 
double *** output::buf_u; 
double *** output::buf_v; 
double *** output::buf_p; 
double *** output::buf_T; 

int output::init(FlowField & flow) {
    NativeArray2D(buf, domain::inpt, domain::jnpt);     
    NativeArray2D(buf_u, domain::inpt, domain::jnpt);     
    NativeArray2D(buf_v, domain::inpt, domain::jnpt);     
    NativeArray2D(buf_p, domain::inpt, domain::jnpt);     
    NativeArray2D(buf_T, domain::inpt, domain::jnpt);     
    for(int i=domain::ibeg; i<domain::iend; i++) {
        for(int j=domain::jbeg; j<domain::jend; j++) {
            buf_u[i-domain::stencilsize][j-domain::stencilsize] = &flow.vars.Get(i,j).u_;
            buf_v[i-domain::stencilsize][j-domain::stencilsize] = &flow.vars.Get(i,j).v_;
            buf_p[i-domain::stencilsize][j-domain::stencilsize] = &flow.vars.Get(i,j).p_;
            buf_T[i-domain::stencilsize][j-domain::stencilsize] = &flow.vars.Get(i,j).T_;
        }
    }
}


int output::WriteFlowdata(int step) {
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << step;
    std::string filename = std::string("flowdata_") + oss.str() + std::string(".h5");

    int rank = 2;
    hsize_t current_dims[2] = {domain::inpt, domain::jnpt};

    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dspace_id = H5Screate_simple(rank, current_dims, NULL);
    
    hid_t dset_id = H5Dcreate(file_id, "u", H5T_NATIVE_DOUBLE, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    for(int i=0; i<domain::inpt; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            buf[i][j] = *buf_u[i][j];
        }
    }
    herr_t herr = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buf);
    herr = H5Dclose(dset_id);
    
    dset_id = H5Dcreate(file_id, "v", H5T_NATIVE_DOUBLE, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    for(int i=0; i<domain::inpt; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            buf[i][j] = *buf_v[i][j];
        }
    }
    herr = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buf);
    herr = H5Dclose(dset_id);
    
    dset_id = H5Dcreate(file_id, "p", H5T_NATIVE_DOUBLE, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    for(int i=0; i<domain::inpt; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            buf[i][j] = *buf_p[i][j];
        }
    }
    herr = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buf);
    herr = H5Dclose(dset_id);
    

    dset_id = H5Dcreate(file_id, "T", H5T_NATIVE_DOUBLE, dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    for(int i=0; i<domain::inpt; i++) {
        for(int j=0; j<domain::jnpt; j++) {
            buf[i][j] = *buf_T[i][j];
        }
    }
    herr = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buf);
    herr = H5Dclose(dset_id);
    
herr = H5Sclose(dspace_id);
    herr = H5Fclose(file_id);

}

