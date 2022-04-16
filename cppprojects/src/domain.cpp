#include "domain.hpp"

Domain::Domain(const int nx, const int ny, const int nz, const int stencil, 
               const int ibegbc, const int iendbc, 
               const int jbegbc, const int jendbc, const int kbegbc, const int kendbc) {
    stencil_ = stencil;

    int stencil_worker;
   
    if(nx>1) {
        stencil_worker = stencil_;
    } 
    else {
        stencil_worker = 0;
    }
    ilen_global_ = nx;
    ibeg_global_ = stencil_worker;
    iend_global_ = nx + stencil_worker - 1;
    ibeg_ = ibeg_global_;
    iend_ = iend_global_;
    ilen_ = ilen_global_;
    ihalobeg_ = 0;
    ihaloend_ = iend_ + stencil_worker;
     

    if(ny>1) {
        stencil_worker = stencil_;
    } 
    else {
        stencil_worker = 0;
    }
    jlen_global_ = ny;
    jbeg_global_ = stencil_worker;
    jend_global_ = ny + stencil_worker - 1;
    jbeg_ = jbeg_global_;
    jend_ = jend_global_;
    jlen_ = jlen_global_;
    jhalobeg_ = 0;
    jhaloend_ = jend_ + stencil_worker;


    if(nz>1) {
        stencil_worker = stencil_;
    } 
    else {
        stencil_worker = 0;
    }
    klen_global_ = nz;
    kbeg_global_ = stencil_worker;
    kend_global_ = nz + stencil_worker - 1;
    kbeg_ = kbeg_global_;
    kend_ = kend_global_;
    klen_ = klen_global_;
    khalobeg_ = 0;
    khaloend_ = kend_ + stencil_worker;

    SetBC(ibegbc, iendbc, jbegbc, jendbc, kbegbc, kendbc);

    
}

int Domain::SetBC(const int ibegbc, const int iendbc, const int jbegbc, const int jendbc, const int kbegbc, const int kendbc) {
    ibegbc_ = ibegbc;
    iendbc_ = iendbc;
    jbegbc_ = jbegbc;
    jendbc_ = jendbc;
    kbegbc_ = kbegbc;
    kendbc_ = kendbc;

    switch(ibegbc_) {
        case (0):
            std::cout << "Periodic in x-begin" << std::endl;
            break;
    }

    return 0;
}




int Domain::Partition(const int num_points, const int num_segments, int* num_points_per_segment, int* offset){
    int base = num_points / num_segments;
    for(int count=0; count<num_segments; ++count) {
        num_points_per_segment[count] = base;
    }

    /// distribute extra 
    int extra = num_points - base*num_segments;
    for( ; extra>0; extra--){
        num_points_per_segment[extra-1] += 1;
    }

    /// get offset
    offset[0] = 0;
    for(int count=1; count<num_segments; ++count){
        offset[count] = offset[count-1] + num_points_per_segment[count-1];
    }
    
    return 0;

}



