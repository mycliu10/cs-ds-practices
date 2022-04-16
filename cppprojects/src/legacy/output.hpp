#pragma once
#include "hdf5.h"
#include "domain.hpp"
#include "everything.hpp"

class output {
public:
    static double ** buf;
    static double *** buf_u;
    static double *** buf_v;
    static double *** buf_p;
    static double *** buf_T;
//    output(const FlowField & flow);
    static int init(FlowField & flow);
    static int WriteFlowdata(int step); 
    


};
