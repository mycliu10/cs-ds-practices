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

        Builder() : dimension(0), dimensions({ 3, maxDimension}) {}

        Builder & addDimension(int haloBegin, int length, int haloEnd) {
            dimensions.setElement({0, dimension}, haloBegin);
            dimensions.setElement({1, dimension}, haloBegin + length);
            dimensions.setElement({2, dimension}, haloBegin + length + haloEnd);
            ++dimension;
            return *this;
        }

        Index build() {
            return Index(*this);
        }
    };

private:
    Index(Builder& builder) : dimension(builder.dimension),
            dimensions({ 3, builder.dimension }) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < 3; ++j) {
                dimensions.setElement({ j, i }, builder.dimensions.getElement({ j, i }));
            }
        }
    }

public:
    const vector<int> getSizes() const {
        vector<int> sizes(dimension);
        for(int i = 0; i < dimension; ++i) {
            sizes[i] = dimensions.getElement({ 2, i });
        }
        return sizes;
    }

    int const getDimension() const {
        return dimension;
    }

    int getHaloBegin(int dimension) {
        return 0;
    }

    int getBegin(int dimension) {
        return dimensions.getElement({ 0, dimension });
    }

    int getEnd(int dimension) {
        return dimensions.getElement({ 1, dimension });
    }

    int getHaloEnd(int dimension) {
        return dimensions.getElement({ 2, dimension });
    }

    int getNumPoints(int dimension) {
        return getEnd(dimension) - getBegin(dimension);
    }

    int getNumCell(int dimension) {
        return getNumPoints(dimension) - 1;
    }

    IntGenerator getHaloBeginZone(int dimension) {
        return IntGenerator(getHaloBegin(dimension), getBegin(dimension));
    }

    IntGenerator getInteriorZone(int dimension) {
        return IntGenerator(getBegin(dimension), getEnd(dimension));
    }
    
    IntGenerator getHaloEndZone(int dimension) {
        return IntGenerator(getEnd(dimension), getHaloEnd(dimension));
    }

    IntGenerator getAllZone(int dimension) {
        return IntGenerator(getHaloBegin(dimension), getHaloEnd(dimension));
    }
};
