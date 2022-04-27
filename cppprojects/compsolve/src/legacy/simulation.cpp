#include "simulation.hpp"

Simulation::Simulation() {
}

Simulation::Initialize() {
    nx_ = 100;
    ny_ = 100;
    inum_procs_ = 1;
    jnum_procs_ = 1;
    fluid_type_ = 0;

    time_ = 0.;
    dt_ = 0.;
    CFL_number_ = 0.3;

    bc_ibeg_ = 0;
    bc_iend_ = 0;
    bc_jbeg_ = 0;
    bc_jend_ = 0;

    flow_box_.domain_.Initialize(nx_, ny_, inum_procs_, jnum_procs_);
    flow_box_.timer_.Initialize(time_, CFL_number_, dt_);
    flow_box_.boundary_conditions_.Initialize(bc_ibeg_, bc_iend_, bc_jbeg_, bc_jend_);



    step_ = 0;
    max_steps_ = 1000;

}
