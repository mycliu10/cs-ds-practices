#pragma once

class OSDFamily {
protected:
    int derivative_order_;
    int order_of_accuracy_;
    int stencil_;
    double coefs_[8];
public:
    OSDFamily();
    double Kernel(double *const f);
};


class OSD1stFamily : public OSDFamily {
public:
    OSD1stFamily(const int order_of_accuracy);
};

class OSD2ndFamily : public OSDFamily {
public:
    OSD2ndFamily(const int order_of_accuracy);
};
