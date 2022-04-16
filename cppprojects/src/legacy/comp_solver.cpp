#include <iostream>
#include "simulation.hpp"

using namespace std;

int main(){
    int shape[2] = {194, 32};
    int nproc[2] = {1, 1};
    int err;
    double dt = 1.e-6;
    
    ///
    Simulation simulation;
    simulation.Initialize()

    simulation.Run()

    return 1;
}
