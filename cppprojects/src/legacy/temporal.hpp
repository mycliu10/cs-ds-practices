#pragma once
#include "everything.hpp"
#include "BC.hpp"

class temporal{
public:
    static double dt;

    static int rk4();
    static int Euler(FlowField & flow, int (*step)(FlowField & flow, BoundaryCondition & BC), BoundaryCondition & BC, double & dt);

    static int check_CFL(FlowField & flow, double & CFL);

    static int constant_CFL(FlowField & flow, const double CFL, double & dt);

};
