#pragma once
#include "flow_box.hpp"

class Simulation {
public:
    int nx_, ny_;
    int inum_procs_, jnum_procs_;
    int fluid_type_;

    int temporal_scheme_; 
    double dt_;
    double CFL_number_;

    int step_;
    int max_steps_;
   
    FlowBox flow_box_;

    Simulation();

    int Initialize();
    int Run();
};

