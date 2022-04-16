#include "ideal_gas.hpp"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

IdealGas::IdealGas() {
}

IdealGas::IdealGas(const double R, const double gamma, const double Pr) {
    Initialize(R, gamma, Pr);
}

int IdealGas::Initialize(const double R, const double gamma, const double Pr) {
    R_ = R;
    gamma_ = gamma;
    Pr_ = Pr;
    Cv_ = R_/(gamma_-1.);
    Cp_ = Cv_ + R_;
    cout << "R = " << R_ << endl;
    cout << "gamma = " << gamma_ << endl;
    cout << "Cv = " << Cv_ << endl;
    cout << "Cp = " << Cp_ << endl;
    cout << "Pr = " << Pr_ << endl;
    cout << "Initialized ideal gas." << endl;
    cout << "========================================================" << endl;
    return 1;
}



double IdealGas::GetDynamicViscositySutherland(const double  T) const {
    const double CS = 1.458e-6;
    const double TS = 110.4;
    const double OS = 0.5;

    double mu = CS * pow(T, 1.+OS) / (T+TS);
    return mu;
}


double IdealGas::GetThermalConductivity(const double mu) const {
    double kappa = Cp_*mu / Pr_;
    return kappa;
}

    
