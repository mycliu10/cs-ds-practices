#include "head.hpp"

class Scheme {
protected:
    int orderOfAccuracy;
    int stencilSize;
    int offset;
public:
    Scheme() {}

    double virtual compute(vector<double> f) = 0;

    IntGenerator getStencilGenerator() {
        return IntGenerator(offset, offset + stencilSize);
    }
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
    double compute(vector<double> f) {
        return f[2] - 2.*f[1] + f[0];
    }
};
