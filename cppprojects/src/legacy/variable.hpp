#pragma once

class Variable {
public:
    double rho_, rhou_, rhov_, rhoE_, E_, u_, v_, p_, T_, mu_, kappa_, a_;
    double rhouu_, rhouv_, rhovv_, rhouE_, rhovE_;
    double rhoh_, rhouh_, rhovh_;

    variable();

    int Get_everything_from_primitive();
    int Get_everything(); 
    int CopyAll(const variable & var);
};



