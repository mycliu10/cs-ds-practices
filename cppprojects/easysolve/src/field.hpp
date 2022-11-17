#include "index.hpp"

class Field {
    Index* index;
    VectorN<double> field;

public:
    Field(Index& meshIndex) {
        index = &meshIndex;
        field.resize(index->getSizes());
    }

    double& element(vector<int> point) {
        return field.element(point);
    }
};
