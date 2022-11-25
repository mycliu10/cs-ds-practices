#include "head.hpp"

class Index {
    int dimension;
    VectorN<int> dimensions;

public:
    class Builder {
    public:
        int dimension;
        VectorN<int> dimensions;
        static int const maxDimension = 8;

        Builder() : dimension(0), dimensions({ maxDimension, 3 }) {}

        Builder & addDimension(int haloBegin, int length, int haloEnd) {
            dimensions.setElement({ dimension, 0 }, haloBegin);
            dimensions.setElement({ dimension, 1 }, haloBegin + length);
            dimensions.setElement({ dimension, 2 }, haloBegin + length + haloEnd);
            ++dimension;
            return *this;
        }

        Index * build() {
            return new Index(*this);
        }
    };

private:
    Index(Builder& builder) : dimension(builder.dimension),
            dimensions({ builder.dimension, 3 }) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < 3; ++j) {
                dimensions.setElement({ i, j }, builder.dimensions.getElement({ i, j }));
            }
        }
    }

public:
    vector<int> getSizes() const {
        vector<int> sizes(dimension);
        for(int i = 0; i < dimension; ++i) {
            sizes[i] = dimensions.getElement({ i, 2 });
        }
        return sizes;
    }

    int getDimension() const {
        return dimension;
    }

    int getHaloBegin() {
        return 0;
    }

    int getBegin(int dimension) {
        return dimensions.getElement({ dimension, 0 });
    }

    int getEnd(int dimension) {
        return dimensions.getElement({ dimension, 1 }) - 1;
    }

    int getHaloEnd(int dimension) {
        return dimensions.getElement({ dimension, 2 }) - 1;
    }

    int getNumPoints(int dimension) {
        return getEnd(dimension) - getBegin(dimension);
    }

    int getNumCell(int dimension) {
        return getNumPoints(dimension) - 1;
    }

    IntGenerator getHaloBeginZone(int dimension) {
        return IntGenerator(getHaloBegin(), getBegin(dimension));
    }

    IntGenerator getInteriorZone(int dimension) {
        return IntGenerator(getBegin(dimension), getEnd(dimension) + 1);
    }
    
    IntGenerator getHaloEndZone(int dimension) {
        return IntGenerator(getEnd(dimension), getHaloEnd(dimension) + 1);
    }

    IntGenerator getAllZone(int dimension) {
        return IntGenerator(getHaloBegin(), getHaloEnd(dimension));
    }
};
