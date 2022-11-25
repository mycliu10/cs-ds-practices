#pragma once
#include <initializer_list>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::initializer_list;
using std::vector;

template <class T> 
class VectorN {
    vector<int> dimensions;
    vector<T> data;

public:
    VectorN<T>() {}
        
    VectorN<T>(vector<int> sizes) {
        this->resize(sizes);
    }

    void resize(vector<int> sizes) {
        dimensions.clear();
        data.clear();
        if (sizes.size() < 1) {
        }
        int maxsize = 1;
        for(int n : sizes) {
            if (n < 1) {
            }
            dimensions.push_back(n);
            maxsize *= n;
        }
        data.resize(maxsize);
    }

    void reset(T value) {
        for(auto & element : data) {
            element = value;
        }
    }

    T & element(initializer_list<int> & point) {
        int n = *point.begin();
        for(auto i = point.begin() + 1; i != point.end(); ++i) {
            n *= dimensions[i-point.begin()-1];
            n += *i;
        }
        return data[n];
    }

    T & element(vector<int> & point) {
        int n = point[0];
        for(int i = 1; i < int(point.size()); ++i) {
            n *= dimensions[i-1];
            n += point[i];
        }
        return data[n];
    }

    T getElement(vector<int> const & point) const {
        int n = point[0];
        for(int i = 1; i < int(point.size()); ++i) {
            n *= dimensions[i-1];
            n += point[i];
        }
        return data[n];
    }

    void setElement(vector<int> const & point, T const value) {
        int n = point[0];
        for(int i = 1; i < int(point.size()); ++i) {
            n *= dimensions[i-1];
            n += point[i];
        }
        data[n] = value;
    }

    VectorN<T> & operator=(VectorN<T> & v) {
        if (this == &v) {
            return *this;
        }
        dimensions = v.dimensions;
        data = v.data;
        return *this;
    }
};


class IntGenerator {
    int begin;
    int end;
    int step;
    int current;

public:
    IntGenerator(int begin, int end) : begin(begin), end(end) {
        step = 1;
        current = begin;
    }

    void reset() {
        current = begin;
    }

    bool isValid() {
        if (current >= end) {
            return false;
        }
        return true;
    }

    void next() {
        current += step;
        
    }

    int getCurrent() const {
        return current;
    }

    int getCount() {
        return current - begin;
    }
};

