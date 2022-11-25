#include "index.hpp"
#include "boundary_condition.hpp"

class Field {
protected:
    Index * index;
    BoundaryConditionsSet * boundaryConditionsSet;
    VectorN<double> data;

public:
    Field() {}

    Field(Index * index, BoundaryConditionsSet * boundaryConditionsSet) {
        resize(index, boundaryConditionsSet);
    }

    void resize(Index * index, BoundaryConditionsSet * boundaryConditionsSet) {
        this->index = index;
        this->boundaryConditionsSet = boundaryConditionsSet;
        data.resize(index->getSizes());
        data.reset(0.);
    }

    double & element(vector<int> & point) {
        return data.element(point);
    }

    double getElement(vector<int> const & point) const {
        return data.getElement(point);
    }

    void setElement(vector<int> const & point, double const value) {
        data.setElement(point, value);
    }

    BoundaryCondition * getBeginBoundaryCondition(int dimension) {
        return boundaryConditionsSet->getBeginBoundaryCondition(dimension);
    }

    BoundaryCondition * getEndBoundaryCondition(int dimension) {
        return boundaryConditionsSet->getEndBoundaryCondition(dimension);
    }

    Field & operator=(Field & f) {
        if (this == &f) {
            return *this;
        }
        index = f.index;
        data = f.data;
        boundaryConditionsSet = f.boundaryConditionsSet;
        return *this;
    }
};
