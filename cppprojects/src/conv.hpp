#pragma once
#include "problem.hpp"
#include "array.hpp"
#include "ideal_gas.hpp"
#include "domain.hpp"
#include "mesh.hpp"
#include "flow.hpp"
#include "RHS.hpp"
#include "characteristic.hpp"
#include "split.hpp"
#include "average.hpp"
#include "WENO.hpp"

class ConvectionRun {
private:
    Problem * p_;

    Transformer * transformer_;
    Averager * averager_;
    Splitter * splitter_;
    Scheme * scheme_;
    int do_reconstruction_;
    int do_characteristic_;
    int do_split_;

    template <class It>
    int RunLine(const int direction, 
                It iter_uu, It iter_fu,
                It iter_p,  It iter_a,
                It iter_u,  It iter_v, It iter_w,
                It iter_ustar, It iter_ustarma, It iter_ustarpa, 
                It iter_fhat);
                
public:
    ConvectionRun(Problem *const p,      Averager *const averager,
                  Transformer *const transformer, Splitter *const splitter, 
                  Scheme *const scheme) {
        p_ = p;
        transformer_ = transformer;
        averager_ = averager;
        splitter_ = splitter;
        scheme_ = scheme;

        // initialize control flags
        do_reconstruction_ = 0;
        do_characteristic_ = 0;
        do_split_ = 0;
        if(averager_) {
            do_reconstruction_ = 1;
            std::cout << "ConvectionRun reconstruction ON." << std::endl;
        }
        if(transformer_) {
            do_characteristic_ = 1;
            std::cout << "ConvectionRun characteristic ON." << std::endl;
        }
        if(splitter_) {
            do_split_ = 1;
            std::cout << "ConvectionRun split          ON." << std::endl;
        }

        std::cout << "ConvectionRun initialized." << std::endl;
        std::cout << "========================================================" << std::endl;
    }

    int RunX();
    int RunY();
    int RunZ();

};

 
