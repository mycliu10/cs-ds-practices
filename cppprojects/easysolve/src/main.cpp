#include "problem.hpp"


int main() {
    Timer::start("Main");

    shared_ptr<PoissonProblem> problem(new PoissonProblem());

    shared_ptr<BoundaryCondition> westBoundaryCondition(new DirichletBoundaryCondition(1.));
    shared_ptr<BoundaryCondition> eastBoundaryCondition(new NeumannBoundaryCondition (0.));
    shared_ptr<BoundaryCondition> dummyBoundaryCondition(new DummyBoundaryCondition());

    shared_ptr<BoundaryConditionsSet> meshBoundaryConditions(BoundaryConditionsSet::Builder(1)
            .setBeginBoundaryCondition(dummyBoundaryCondition, 0)
            .setEndBoundaryCondition(dummyBoundaryCondition, 0)
            .build());

    shared_ptr<BoundaryConditionsSet> temperatureBoundaryConditions(BoundaryConditionsSet::Builder(1)
            .setBeginBoundaryCondition(westBoundaryCondition, 0)
            .setEndBoundaryCondition(eastBoundaryCondition, 0)
            .build());

    int numPoints = 16;
    double lx = 1.;
    double dt = (lx / numPoints) * (lx / numPoints) * 0.1;

    shared_ptr<Index> index(Index::Builder()
            .addDimension(0, numPoints, 1).
            build());

    shared_ptr<Mesh> mesh(new UniformMesh(index, meshBoundaryConditions, lx));

    shared_ptr<Advancement> advancement(new FirstOrderForwardEuler());

    problem->initialize(mesh, advancement, temperatureBoundaryConditions);

    problem->solve(dt, 10000000);


    Timer::stop("Main");

    Timer::report();
}
