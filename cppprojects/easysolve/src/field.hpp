#include "index.hpp"

class Field {
protected:
    Index * index;
    VectorN<double> data;

public:
    Field() : index(NULL), data() {}

    Field(Index * index) : index(index), data(index->getSizes()) {}

    void resize(Index * index) {
        this->index = index;
        data.resize(index->getSizes());
    }

    double getElement(vector<int> const & point) const {
        return data.getElement(point);
    }

    void setElement(vector<int> const & point, double const value) {
        data.setElement(point, value);
    }

    Field & operator=(Field & f) {
        if (this == &f) {
            return *this;
        }
        index = f.index;
        data = f.data;
        return *this;
    }
};
