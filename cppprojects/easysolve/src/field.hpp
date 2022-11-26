#include "index.hpp"
#include "boundary_condition.hpp"

class Field {
protected:
    shared_ptr<Index> index;
    shared_ptr<BoundaryConditionsSet> boundaryConditionsSet;
    VectorN<double> data;

public:
    Field() {}

    Field(shared_ptr<Index> index, shared_ptr<BoundaryConditionsSet> boundaryConditionsSet) {
        resize(index, boundaryConditionsSet);
    }

    void resize(shared_ptr<Index> index, shared_ptr<BoundaryConditionsSet> boundaryConditionsSet) {
        this->index = index;
        this->boundaryConditionsSet = boundaryConditionsSet;
        data.resize(index->getSizes());
        data.reset(0.);
    }

    double & element(initializer_list<int> point) {
        return data.element(point);
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

    shared_ptr<BoundaryCondition> getBoundaryCondition(BoundaryConditionSide side, int dimension) {
        return boundaryConditionsSet->getBoundaryCondition(side, dimension);
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
