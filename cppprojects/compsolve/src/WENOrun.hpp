#pragma once
#include "array.hpp"
#include "domain.hpp"
#include "mesh.hpp"
#include "flow.hpp"
#include "RHS.hpp"
#include "WENO.hpp"

class WENORun {
private:
    Array<double> uu_worker_;
    Array<double> fu_worker_;
    Array<double> fhat_worker_;
    Array<double> a_worker_;
    Array<double> ustar_worker_;
    Domain * domain_;
    Mesh * mesh_;
    Flow * flow_;
    RHS * rhs_;

    
public:

};
