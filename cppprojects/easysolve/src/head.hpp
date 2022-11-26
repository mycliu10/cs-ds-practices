#pragma once
#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

using std::cout;
using std::endl;
using std::initializer_list;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;
using std::unordered_map;

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


class Timer {
    static unordered_map<string, double> starts;
    static unordered_map<string, double> durations;

    Timer() {}

public:
    static double getNow() {
        auto ts = std::chrono::high_resolution_clock::now();
        auto s = std::chrono::duration_cast<std::chrono::seconds>(ts.time_since_epoch());
        return s.count();
    }

    static void start(string name) {
        starts[name] = getNow();
    }

    static void stop(string name) {
        durations[name] += getNow() - starts[name];
    }

    static void report() {
        for (auto & duration : durations) {
            cout << duration.first << ": " << duration.second << " s" << endl;
        }
    }
};
unordered_map<string, double> Timer::starts;
unordered_map<string, double> Timer::durations;