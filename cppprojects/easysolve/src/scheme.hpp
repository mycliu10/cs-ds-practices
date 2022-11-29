#include "head.hpp"

class Scheme {
protected:
    int orderOfAccuracy;
    int stencilSize;
    int offset;
public:
    Scheme() {}

    double virtual compute(vector<double> & f) = 0;

    double virtual compute(vector<double>::iterator f) = 0;

    int getStencilSize() {
        return stencilSize;
    }

    int getOffset() {
        return offset;
    }

    IntGenerator getStencilGenerator() {
        return IntGenerator(offset, offset + stencilSize);
    }

    virtual ~Scheme() {}
};


class SecondOrderDerivativeScheme : public Scheme {
};


class CentralDifferenceSecondOrder : public SecondOrderDerivativeScheme {
public:
    CentralDifferenceSecondOrder() {
        orderOfAccuracy = 2;
        stencilSize = 3;
        offset = -1;
    }

    double compute(vector<double> & f) {
        return f[2] - 2. * f[1] + f[0];
    }

    double compute(vector<double>::iterator f) {
        return *(f+2) - 2. * (*(f+1)) + *f;
    }
};
