#pragma once
#include "everything.hpp"
#include "eigeninflow.hpp"


class BoundaryConditionJ {
public:
    int Periodic(FlowField & flow);

};


class BoundaryConditionI {
public:
    EigenInflow eigen_inflow_;
    int ExtrapolateZerothOrderEnd(FlowField & flow);
    int ForceEigenInflow(FlowField & flow);

};



class BoundaryConditions {
public:
    int bc_ibeg_, bc_iend_;
    int bc_jbeg_, bc_jend_;
};



