#pragma once

class IdealGas {
public:
    double Cv_, Cp_, R_, gamma_, Pr_;
    
    IdealGas();
    IdealGas(const double R, const double gamma, const double Pr);
    int Initialize(const double R, const double gamma, const double Pr);

    int Get_p_and_a(const double & rho, const double & T, double & p, double & a);
    int Get_rho_and_a(const double & rho, const double & T, double & p, double & a);
    
    double GetDynamicViscositySutherland(const double T) const;
    double GetThermalConductivity(const double mu) const;

};
