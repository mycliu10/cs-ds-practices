#pragma once
#ifdef _USE_OMP
#include <omp.h>
#endif

#include "array.hpp"

class Domain {
private:
    int SetBC(const int ibegbc, const int iendbc, 
              const int jbegbc, const int jendbc, 
              const int kbegbc, const int kendbc);
public:
    int myid_;
    int ipid_, imid_; /// i plus id ...
    int jpid_, jmid_;

    int ompid_;
    int num_omp_threads_;

    int stencil_;
    int ibeg_, iend_, ilen_;
    int jbeg_, jend_, jlen_;
    int kbeg_, kend_, klen_;
    int ibeg_global_, iend_global_, ilen_global_;
    int jbeg_global_, jend_global_, jlen_global_;
    int kbeg_global_, kend_global_, klen_global_;

    int ihalobeg_, ihaloend_;
    int jhalobeg_, jhaloend_;
    int khalobeg_, khaloend_;

    int ibegbc_, iendbc_;
    int jbegbc_, jendbc_;
    int kbegbc_, kendbc_;

    Domain(const int nx, const int ny, const int nz, const int stencil, const int ibegbc, const int iendbc, const int jbegbc, const int jendbc, const int kbegbc, const int kendbc);

    // Note that    periodic    0
    //              wall        1
    inline int SetOMP() {
#ifdef _USE_OMP
        num_omp_threads_ = omp_get_num_threads();
        ompid_ = omp_get_thread_num();
        if(ompid_ == 0) {
            cout << "OMP_NUM_THREADS = " << num_omp_threads_ << endl;
        }
#endif
        return 0;
    }

private:
    int Partition(const int num_points, const int num_segments, int* num_points_per_segment, int* offset);
};


