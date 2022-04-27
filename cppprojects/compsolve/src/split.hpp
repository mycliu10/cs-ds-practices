#pragma once
#include <iostream>
#include <algorithm>
#include "worker.hpp"

class Splitter {
private:

public:
    Splitter() {
        std::cout << "LFSplitter initialized." << std::endl;
        std::cout << "========================================================" << std::endl;
    }

    virtual ~Splitter() {}

    virtual int Split(vector<double>::iterator it_u, 
                      vector<double>::iterator it_upa,
                      vector<double>::iterator it_uma,
                      vector<double>::iterator it_uu,
                      vector<double>::iterator it_fu,
                      vector<double>::iterator it_fp,
                      vector<double>::iterator it_fm,
                      const int N) = 0;

    virtual int Merge(double fp,
                      double fm,
                      double & f) = 0;

    virtual int MergeDerivative(double fp,
                                double fm,
                                double & f) = 0;
};



class LFSplitter : public Splitter {
public:
    LFSplitter() {}

    int Split(vector<double>::iterator it_u, 
              vector<double>::iterator it_upa,
              vector<double>::iterator it_uma,
              vector<double>::iterator it_uu,
              vector<double>::iterator it_fu,
              vector<double>::iterator it_fp,
              vector<double>::iterator it_fm,
              const int N) {
        double alpha_u   = *std::max_element(it_u,   it_u  +N);
        double alpha_upa = *std::max_element(it_upa, it_upa+N);
        double alpha_uma = *std::max_element(it_uma, it_uma+N);
    
        for(int i=0; i<N; ++i) {
            *(it_fp+      i) = .5 * (*(it_fu+    i) + alpha_uma * (*(it_uu+    i)));
            *(it_fm+  N-1-i) = .5 * (*(it_fu+    i) - alpha_uma * (*(it_uu+    i)));
            *(it_fp+  N  +i) = .5 * (*(it_fu+  N+i) + alpha_u   * (*(it_uu+  N+i)));
            *(it_fm+2*N-1-i) = .5 * (*(it_fu+  N+i) - alpha_u   * (*(it_uu+  N+i)));
            *(it_fp+2*N  +i) = .5 * (*(it_fu+2*N+i) + alpha_u   * (*(it_uu+2*N+i)));
            *(it_fm+3*N-1-i) = .5 * (*(it_fu+2*N+i) - alpha_u   * (*(it_uu+2*N+i)));
            *(it_fp+3*N  +i) = .5 * (*(it_fu+3*N+i) + alpha_u   * (*(it_uu+3*N+i)));
            *(it_fm+4*N-1-i) = .5 * (*(it_fu+3*N+i) - alpha_u   * (*(it_uu+3*N+i)));
            *(it_fp+4*N  +i) = .5 * (*(it_fu+4*N+i) + alpha_upa * (*(it_uu+4*N+i)));
            *(it_fm+5*N-1-i) = .5 * (*(it_fu+4*N+i) - alpha_upa * (*(it_uu+4*N+i)));
        }
        return 0;
    }

    int Merge(double fp, double fm, double& f) {
        f = fp + fm;
        return 0;
    }

    int MergeDerivative(double fp, double fm, double& f) {
        f = fp - fm;
        return 0;
    }

};


