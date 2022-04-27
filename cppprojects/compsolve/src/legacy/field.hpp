#pragma once
#include "array.hpp"

template <class A>
class field {
public:
    array2d<A> data;
    
    field(int ilen, int jlen);
};


    
