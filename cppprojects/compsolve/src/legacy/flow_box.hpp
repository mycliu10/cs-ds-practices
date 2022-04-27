#pragma once
#include <iostream>
#include <cmath>
#include "array.hpp"
#include "domain.hpp"
//#include "baseflow.hpp"
#include "mesh_point.hpp"
#include "flow_point.hpp"

class FlowBox {
public:
    Domain domain_;
    Timer timer_;

    Array2D<MeshPoint> mesh_points_;
    
    Array2D<FlowPoint> flow_points_;

    Array2D<ConvectionPoint> convection_points_;
    Array2D<ViscousPoint> viscous_points_;
    Array2D<RHSPoint> rhs_points_;

    BoundaryConditions boundary_conditions_;
//    BaseFlow baseflow_;
    
    FlowBox();


    int Initialize();


    int SolveBoundaryConditions();
    int SolveViscous();
    int SolveConvection();
    int UpdateRHS();
    int UpdateFlow();

    int EvaluateCFLNumber();
    int EvaluateResidual();
};



    
