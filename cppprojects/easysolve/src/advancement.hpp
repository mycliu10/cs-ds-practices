#pragma once
#include "head.hpp"

class Advancement {
protected:
    Index * index;

public:
    virtual double advance(double & f, double & rhs, double dt) = 0;

    virtual ~Advancement() {}
};


class FirstOrderForwardEuler : public Advancement {
public:
    double advance(double & f0, double & rhs, double dt) {
        return f0 + rhs * dt;
    }
};
