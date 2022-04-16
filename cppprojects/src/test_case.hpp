#pragma once
#include "mesh.hpp"
#include "flow.hpp"

class TestCase {
protected:
public:
    TestCase() {}

    virtual int SetDomain(Domain &domain) = 0;
    virtual int SetMesh(Mesh &mesh) = 0;
    virtual int SetFlow(Flow &flow) = 0;
};



class ConvectiveVortexTestCase : public TestCase {
private:
    double rv_;
    double Lx_;
    double Ly_;
    double Lz_;
    double Minf_;
    double Mv_;
    double pinf_;
    double Tinf_;
    double rhoinf_;
    double ainf_;
    double uinf_;
public:
    ConvectiveVortexTestCase() {
        rv_ = 1.;
        Lx_ = 24. * rv_;
        Ly_ = 12. * rv_;
        Lz_ = 12. * rv_;
        Minf_ = 0.5;
        Mv_ = 0.4;
        pinf_ = 500.;
        Tinf_ = 50.;
    }

    int SetDomain(Domain &domain) {

    }

    int SetMesh(Domain &domain, Mesh &mesh) {
        int ibeg = domain.ibeg_;
        int jbeg = domain.jbeg_;
        int kbeg = domain.kbeg_;
        int iend = domain.iend_;
        int jend = domain.jend_;
        int kend = domain.kend_;
        int ilen = domain.ilen_;
        int jlen = domain.jlen_;
        int klen = domain.klen_;
 
        double dx = Lx_ / double(ilen);
        double dy = Ly_ / double(jlen);
        double dz = Lz_ / double(klen);

        for(int i=ibeg; i<=iend; ++i) {
            for(int k=kbeg; k<=kend; ++k) {
                for(int j=jbeg; j<=jend; ++j) {
                    mesh.x_.Get(i,k,j) = dx * (i-ibeg-ilen/2);
                    mesh.y_.Get(i,k,j) = dy * (j-jbeg-jlen/2);
                    mesh.z_.Get(i,k,j) = dz * (k-kbeg-klen/2);
                }
            }
        }

        return 0;
    }

    int SetFlow(Domain &domain, Mesh &mesh, Flow &flow, IdealGas &gas) {
        double gamma = gas.gamma_;
        double R = gas.R_;
        double rhoinf_ = pinf_ / (R * Tinf_);
        double ainf_ = std::sqrt(gamma * R * Tinf_);
        double uinf_ = Minf_ * ainf_;
        int ibeg = domain.ibeg_;
        int jbeg = domain.jbeg_;
        int kbeg = domain.kbeg_;
        int iend = domain.iend_;
        int jend = domain.jend_;
        int kend = domain.kend_;
        int ilen = domain.ilen_;
        int jlen = domain.jlen_;
        int klen = domain.klen_;
 
        for(int i=ibeg; i<=iend; ++i) {
            for(int k=kbeg; k<=kend; ++k) {
                for(int j=jbeg; j<=jend; ++j) {
                    double x = mesh.x_.GetValue(i,k,j);
                    double z = mesh.z_.GetValue(i,k,j);
                    double rsq = (x*x + z*z) / (rv_*rv_);
                    double u = (1.-Mv_/Minf_*z/rv_*std::exp((1.-rsq)/2.)) * uinf_;
                    double w =     Mv_/Minf_*x/rv_*std::exp((1.-rsq)/2.)  * uinf_;
                    double rho = std::pow(1.-(gamma-1.)/2.*std::pow(Mv_,2.)*std::exp(1.-std::pow(rsq,2.)), 1./(gamma-1.)) * rhoinf_;
                    double p = std::pow(1.-(gamma-1.)/2.*std::pow(Mv_,2.)*std::exp(1.-std::pow(rsq,2.)), gamma/(gamma-1.)) * pinf_;
                    double T = p / (R*rho);
                    flow.u_.Get(i,k,j) = u;
                    flow.v_.Get(i,k,j) = 0.;
                    flow.w_.Get(i,k,j) = w;
                    flow.p_.Get(i,k,j) = p;
                    flow.T_.Get(i,k,j) = T;
                }
            }
        }

        return 0;
    }



};
