#include "problem.hpp"
int main() {
    PoissonProblem * problem = new PoissonProblem();

    BoundaryCondition * westBoundaryCondition = new DirichletBoundaryCondition(1.);
    BoundaryCondition * eastBoundaryCondition = new NeumannBoundaryCondition (0.);
    BoundaryCondition * dummyBoundaryCondition = new DummyBoundaryCondition();

    BoundaryConditionsSet * meshBoundaryConditions = BoundaryConditionsSet::Builder(1)
            .setBeginBoundaryCondition(dummyBoundaryCondition, 0)
            .setEndBoundaryCondition(dummyBoundaryCondition, 0)
            .build();

    BoundaryConditionsSet * temperatureBoundaryConditions = BoundaryConditionsSet::Builder(1)
            .setBeginBoundaryCondition(westBoundaryCondition, 0)
            .setEndBoundaryCondition(eastBoundaryCondition, 0)
            .build();

    int numPoints = 16;
    double lx = 1.;

    Index * index = Index::Builder()
            .addDimension(0, numPoints, 0).
            build();

    Mesh * mesh = new UniformMesh(index, meshBoundaryConditions, lx);

    Advancement * advancement = new FirstOrderForwardEuler();

    problem->initialize(mesh, advancement, temperatureBoundaryConditions);

    problem->solve(0.001);
}
