#pragma once
#include <complex>
#include <iostream>
#include <stdexcept>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>
#include "hdf5.h"
#include "profiler.hpp"

using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::unordered_map;


template <class A>
class Array {
private:
    int ndim_;
    int* shape_;
    vector<A> array_;
 
    int Allocate(const int len);
public:
    int size_;

    Array();
    ~Array() {
        array_.clear();
    }
    int Initialize(const int ilen);
    int Initialize(const int ilen, const int jlen);
    int Initialize(const int ilen, const int jlen, const int klen);
    int Initialize(const int ilen, const int jlen, const int klen, const int nlen);
    int GetIndex(const int i, const int j) const;
    int GetIndex(const int i, const int j, const int k) const;
    int GetIndex(const int i, const int j, const int k, const int n) const;
    A& Get(const int i);
    A& Get(const int i, const int j);
    A& Get(const int i, const int j, const int k);
    A& Get(const int i, const int j, const int k, const int n);
    A const GetValue(const int i) const;
    A const GetValue(const int i, const int j) const;
    A const GetValue(const int i, const int j, const int k) const;
    A const GetValue(const int i, const int j, const int k, const int n) const;
    int Zero();
};



template <class A>
Array<A>::Array() {
    ndim_ = 0;
    shape_ = NULL;
    size_ = 0;
}

template <class A>
int Array<A>::Allocate(const int len) {
    array_.resize(len);
    Zero();
    return 0;
}

template <class A>
int Array<A>::Initialize(const int ilen) {
    if(ndim_ > 0) {
        std::cout << "Initialized array being initialized again!" << std::endl;
        return -1;
    }
    if(ilen<1){
        std::cout << "Wrong shape!" << std::endl;
        exit(-2);
        return 0;
    }
    ndim_ = 1;
    shape_ = new int[ndim_]; 
    shape_[0] = ilen;
    size_ = ilen;
//    array_ = new A[ilen];
    Allocate(ilen);
    return 1;
}
template <class A>
int Array<A>::Initialize(const int ilen, const int jlen){
    if(ndim_ > 0) {
        std::cout << "Initialized array being initialized again!" << std::endl;
        return -1;
    }
    if(ilen<1 or jlen<1){
        std::cout << "Wrong shape!" << std::endl;
        exit(-2);
        return 0;
    }
    ndim_ = 2;
    shape_ = new int[ndim_]; 
    shape_[0] = ilen;
    shape_[1] = jlen;
    size_ = ilen*jlen;
//    array_ = new A[ilen*jlen];
    Allocate(ilen*jlen);
    return 1;
}
template <class A>
int Array<A>::Initialize(const int ilen, const int jlen, const int klen) {
    if(ndim_ > 0) {
        std::cout << "Initialized array being initialized again!" << std::endl;
        return -1;
    }
    if(ilen<1 or jlen<1 or klen<1){
        std::cout << "Wrong shape!" << std::endl;
        exit(-2);
        return 0;
    }
    ndim_ = 3;
    shape_ = new int[ndim_]; 
    shape_[0] = ilen;
    shape_[1] = jlen;
    shape_[2] = klen;
    size_ = ilen*jlen*klen;
//    array_ = new A[ilen*jlen*klen];
    Allocate(ilen*jlen*klen);
    return 1;
}
template <class A>
int Array<A>::Initialize(const int ilen, const int jlen, const int klen, const int nlen) {
    if(ndim_ > 0) {
        std::cout << "Initialized array being initialized again!" << std::endl;
        return -1;
    }
    if(ilen<1 or jlen<1 or klen<1 or nlen<1){
        std::cout << "Wrong shape!" << std::endl;
        exit(-2);
        return 0;
    }
    ndim_ = 4;
    shape_ = new int[ndim_]; 
    shape_[0] = ilen;
    shape_[1] = jlen;
    shape_[2] = klen;
    shape_[3] = nlen;
    size_ = ilen*jlen*klen*nlen;
//    array_ = new A[ilen*jlen*klen*nlen];
    Allocate(ilen*jlen*klen*nlen);
    return 0;
}



template <class A>
inline int Array<A>::GetIndex(const int i, const int j) const {
    int index = i*shape_[1] + j;
    return index;
}
template <class A>
inline int Array<A>::GetIndex(const int i, const int j, const int k) const {
    int index = (i*shape_[1] + j)*shape_[2]+k;
    return index;
}
template <class A>
inline int Array<A>::GetIndex(const int i, const int j, const int k, const int n) const {
    int index = ((i*shape_[1] + j)*shape_[2]+k)*shape_[3]+n;
    return index;
}


template <class A>
inline A& Array<A>::Get(const int i){
//    if(i<0 or i>=size_) {
//        std::string errinfo = "Array size out of bound in Array<>.Get(" + std::to_string(i) + ")";
//        throw std::runtime_error(errinfo);
// 
//    } 
    return array_[i];
}
template <class A>
inline A& Array<A>::Get(const int i, const int j){
    int idx = GetIndex(i, j);
    return Get(idx);
}
template <class A>
inline A& Array<A>::Get(const int i, const int j, const int k) {
    int idx = GetIndex(i, j, k);
    return Get(idx);
}
template <class A>
inline A& Array<A>::Get(const int i, const int j, const int k, const int n) {
    int idx = GetIndex(i, j, k, n);
    return Get(idx);
}


template <class A>
inline A const Array<A>::GetValue(const int i) const {
//    if(i<0 or i>=size_) {
//        std::string errinfo = "Array size out of bound in Array<>.GetValue(" + std::to_string(i) + ")";
//        throw std::runtime_error(errinfo);
//    }
    return array_[i];
}
template <class A>
inline A const Array<A>::GetValue(const int i, const int j) const {
    int idx = GetIndex(i, j);
    return GetValue(idx);
}
template <class A>
inline A const Array<A>::GetValue(const int i, const int j, const int k) const {
    int idx = GetIndex(i, j, k);
    return GetValue(idx);
}
template <class A>
inline A const Array<A>::GetValue(const int i, const int j, const int k, const int n) const {
    int idx = GetIndex(i, j, k, n);
    return GetValue(idx);
}

template <class A>
inline int Array<A>::Zero() {
    for(int i=0; i<size_; ++i) {
        array_[i] = 0.;
    }
    return 0;
}


template <class A>
int NativeArray(A ** &arr, const int ilen, const int jlen){
    arr = new A*[ilen];
    arr[0] = new A[ilen*jlen];
    for(int i=1; i<ilen; i++){
        arr[i] = &arr[0][i*jlen];
    }
    return 1;
}


