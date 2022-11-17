#pragma once
#include <vector>

using std::vector;

template <class T> 
class VectorN {
    vector<int> dimensions;
    vector<T> data;

public:
    void resize(vector<int> sizes) {
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

    T& element(vector<int> point) {
        int n = point[0];
        for(int i = 1; i < point.size(); ++i) {
            n *= dimensions[i-1];
            n += point[i];
        }
        return data[n];
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
        current = begin - step;
    }

    void reset() {
        current = begin;
    }

    bool next() {
        current += step;
        if (current >= end) {
            return false;
        }
        return true;
    }

    int getCurrent() {
        return current;
    }
    int getLast() {
        return current - step;
    }
};

