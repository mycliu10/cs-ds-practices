#include <iostream>
#include "input.hpp"
#include "test_case.hpp"

using std::cout;
using std::endl;

///
///  Please note the order of arrays is                [i,k,j]
///  However, the parameters or arguments often follow [i,j,k]
///

Profiler global_profiler;

int main() {
    // GLOBAL VARIABLES
    global_profiler.Register(0, "Total");
    global_profiler.Register(1, "ConvRunX");
    global_profiler.Register(2, "ConvRunY");
    global_profiler.Register(3, "ConvRunZ");
    global_profiler.Register(4, "BCRunX");
    global_profiler.Register(5, "BCRunY");
    global_profiler.Register(6, "BCRunZ");
    global_profiler.Register(15,"TimeInt");
    global_profiler.Register(16,"Residual");
    global_profiler.Register(17,"EvalCFL");
    global_profiler.Register(20,"Restart");
    global_profiler.Register(99,"Init");

    global_profiler.Start(99);
    // GAS
    Input input;
    IdealGas gas(287., 1.4, 0.7);

    // DOMAIN
    Domain domain = input.PassDomain();

    // NUMERICAL SCHEMES
    Scheme* sconv = input.PassConvScheme();

    Averager* average = input.PassAverager();

    Transformer* transform = input.PassTransformer();

    Splitter* split = input.PassSplitter();

    Scheme* svisc1st = input.PassViscScheme1st();

    Scheme* svisc2nd = input.PassViscScheme2nd();

    // MESH //
    Mesh mesh(&domain);
    mesh.ReadMesh("REST/grid.h5");
    mesh.ExtendMesh();
    mesh.CalculateMetrics();

    // FLOW //
    Flow flow(&gas, &domain);
    RHS rhs(&domain);

    // SUMMARARY OF PROBLEM
    Problem problem(&gas, &domain, &mesh, &flow, &rhs);
    Post post(&problem);
    post.Setup( input.PassRestartSteps(), 
                input.PassResidualSteps(), 
                input.PassEvalCFLSteps() );

    // COMPUTATION
    BCRun bcrun(&problem);
    ConvectionRun crun(&problem, average,
                       transform, split, sconv);
    ViscousRun vrun(&problem, svisc1st, svisc2nd);
    
    DNSRun run(&problem, &bcrun, &crun, &vrun);

    // INTEGRATOR
    TimeIntegrator * timeint = input.PassTimeIntegrator();
    timeint->Setup(&run, &post, input.Passdt(), input.PassCFL());


    flow.ReadFlow("REST/flowdata_00000000.h5");




   
    int run_steps = input.PassRunSteps();
    cout << "Simulation running " << run_steps << " steps..." << endl;
    int start_step = 0;

#pragma omp parallel
{
    domain.SetOMP();
}


    global_profiler.Pause(99);
    global_profiler.Report(99);


    // SIMULATION MAIN BODY
    timeint->Run(start_step, run_steps);
 

    global_profiler.Report();

    // CLEAN UP
    delete sconv;
    delete average;
    delete transform;
    delete split;
    delete svisc1st;
    delete svisc2nd;

}
