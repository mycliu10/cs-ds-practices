#include "head.hpp"

class Index {
    int dimension;
    VectorN<int> dimensions;

public:
    Index() {
        dimension = 0;
        dimensions.resize({ 3, 4 });
    }

    void addDimension(int haloBegin, int len, int haloEnd) {
        dimensions.element({0, dimension}) = haloBegin;
        dimensions.element({1, dimension}) = haloBegin + len;
        dimensions.element({2, dimension}) = haloBegin + len + haloEnd;
        ++dimension;
    }

    vector<int> getSizes() {
        vector<int> sizes;
        for(int i = 0; i < dimension; ++i) {
            sizes.push_back(dimensions.element({2, i}));
        }
        return sizes;
    }

    int getHaloBegin(int dimension) {
        return 0;
    }

    int getBegin(int dimension) {
        return dimensions.element({ 0, dimension });
    }

    int getEnd(int dimension) {
        return dimensions.element({ 1, dimension });
    }

    int getHaloEnd(int dimension) {
        return dimensions.element({ 2, dimension });
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
};
