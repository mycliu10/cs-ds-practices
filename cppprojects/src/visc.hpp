#pragma once
#include "problem.hpp"
#include "mesh.hpp"
#include "flow.hpp"
#include "RHS.hpp"
#include "central_difference.hpp"

class ViscousRun {
private:
    Problem * p_;
    Scheme * scheme_1st_;
    Scheme * scheme_2nd_;
    Array<double> u_i_;
    Array<double> u_j_;
    Array<double> u_k_;
    Array<double> u_ii_;
    Array<double> u_jj_;
    Array<double> u_kk_;
    Array<double> u_ij_;
    Array<double> u_ik_;
    Array<double> v_i_;
    Array<double> v_j_;
    Array<double> v_k_;
    Array<double> v_ii_;
    Array<double> v_jj_;
    Array<double> v_kk_;
    Array<double> v_ij_;
    Array<double> v_jk_;
    Array<double> w_i_;
    Array<double> w_j_;
    Array<double> w_k_;
    Array<double> w_ii_;
    Array<double> w_jj_;
    Array<double> w_kk_;
    Array<double> w_ik_;
    Array<double> w_jk_;
    Array<double> T_i_;
    Array<double> T_j_;
    Array<double> T_k_;
    Array<double> T_ii_;
    Array<double> T_jj_;
    Array<double> T_kk_;
    Array<double> mu_i_;
    Array<double> mu_j_;
    Array<double> mu_k_;
    Array<double> kappa_i_;
    Array<double> kappa_j_;
    Array<double> kappa_k_;
 



public:
    ViscousRun(Problem *const p, Scheme *const scheme_1st, Scheme *const scheme_2nd) {
        p_ = p;
        scheme_1st_ = scheme_1st;
        scheme_2nd_ = scheme_2nd;

        int ilen = p_->domain_->ihaloend_+1;
        int jlen = p_->domain_->jhaloend_+1;
        int klen = p_->domain_->khaloend_+1;
        u_i_ .Initialize(ilen, klen, jlen);
        u_j_ .Initialize(ilen, klen, jlen);
        u_k_ .Initialize(ilen, klen, jlen);
        u_ii_.Initialize(ilen, klen, jlen);
        u_jj_.Initialize(ilen, klen, jlen);
        u_kk_.Initialize(ilen, klen, jlen);
        u_ij_.Initialize(ilen, klen, jlen);
        u_ik_.Initialize(ilen, klen, jlen);

        v_i_ .Initialize(ilen, klen, jlen);
        v_j_ .Initialize(ilen, klen, jlen);
        v_k_ .Initialize(ilen, klen, jlen);
        v_ii_.Initialize(ilen, klen, jlen);
        v_jj_.Initialize(ilen, klen, jlen);
        v_kk_.Initialize(ilen, klen, jlen);
        v_ij_.Initialize(ilen, klen, jlen);
        v_jk_.Initialize(ilen, klen, jlen);

        w_i_ .Initialize(ilen, klen, jlen);
        w_j_ .Initialize(ilen, klen, jlen);
        w_k_ .Initialize(ilen, klen, jlen);
        w_ii_.Initialize(ilen, klen, jlen);
        w_jj_.Initialize(ilen, klen, jlen);
        w_kk_.Initialize(ilen, klen, jlen);
        w_ik_.Initialize(ilen, klen, jlen);
        w_jk_.Initialize(ilen, klen, jlen);

        T_i_ .Initialize(ilen, klen, jlen);
        T_j_ .Initialize(ilen, klen, jlen);
        T_k_ .Initialize(ilen, klen, jlen);
        T_ii_.Initialize(ilen, klen, jlen);
        T_jj_.Initialize(ilen, klen, jlen);
        T_kk_.Initialize(ilen, klen, jlen);

        mu_i_.Initialize(ilen, klen, jlen);
        mu_j_.Initialize(ilen, klen, jlen);
        mu_k_.Initialize(ilen, klen, jlen);

        kappa_i_.Initialize(ilen, klen, jlen);
        kappa_j_.Initialize(ilen, klen, jlen);
        kappa_k_.Initialize(ilen, klen, jlen);

        std::cout << "ViscousRun initialized." << std::endl;
        std::cout << "========================================================" << std::endl;
    }

    int RunX();
    int RunY();
    int RunZ();
    int RunV();
};


