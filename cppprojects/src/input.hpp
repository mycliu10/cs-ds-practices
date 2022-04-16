#pragma once
#include <iostream>
#include <istream>
#include <fstream>
#include "third_party/nlohmann/json.hpp"
#include "domain.hpp"
#include "problem.hpp"
#include "mesh.hpp"
#include "flow.hpp"
#include "RHS.hpp"
#include "ideal_gas.hpp"
#include "WENO.hpp"
#include "upwind.hpp"
#include "characteristic.hpp"
#include "temporal.hpp"



using json = nlohmann::json;

class Input {
private:
    const std::string filename_="Lists.json";
    json jin;
    int conv_method_;
    int visc_method_;
    int time_method_;
    
    template <class T>
    T Pass(json& j, const char *const name, const T backup) {
        T a;
        try {
            a = j[name].get<T>();
        }
        catch (...) {
            a = backup;
        }
        return a;
    }
public:
    Input() {
        std::ifstream ifs(filename_.c_str(), std::ifstream::in);
        ifs >> jin;
        ifs.close();

        std::ofstream ofs("pretty.json");
        ofs << std::setw(4) << jin << std::endl;
        ofs.close();
    }

    Domain PassDomain() {
        int nx = Pass<int>(jin, "nx", 1);
        int ny = Pass<int>(jin, "ny", 1);
        int nz = Pass<int>(jin, "nz", 1);
        int halosize = Pass<int>(jin, "halosize", 4);
        int ibegbc = Pass<int>(jin, "ibegbc", 0);
        int jbegbc = Pass<int>(jin, "jbegbc", 0);
        int kbegbc = Pass<int>(jin, "kbegbc", 0);
        int iendbc = Pass<int>(jin, "iendbc", 0);
        int jendbc = Pass<int>(jin, "jendbc", 0);
        int kendbc = Pass<int>(jin, "kendbc", 0);
        
        Domain domain(nx, ny, nz, halosize, 
                      ibegbc, iendbc, jbegbc, jendbc, kbegbc, kendbc);
        return domain;
    }

    Scheme* PassConvScheme() {
        conv_method_ = Pass<int>(jin, "ConvMethod", 0);
        int order_of_accuracy = Pass<int>(jin, "ConvOA", 5);
        Scheme* sconv;
        switch(conv_method_) {
            case(0):
                switch(order_of_accuracy) {
                    case(5):
                        sconv = new WENOFamily(6, 0);
                        break;
                    case(7):
                        sconv = new WENOFamily(8, 0);
                        break;
                    default:
                        break;
                }
                break;
            case(1):
                sconv = new CDFamily(1, 2);
                break;
        }

        return sconv;
    }

    Scheme * PassViscScheme1st() {
        visc_method_ = Pass<int>(jin, "ViscMethod", 0);
        int order_of_accuracy = Pass<int>(jin, "ViscOA", 2);
        Scheme * svisc = NULL;
        switch(visc_method_) {
            case(0):
                switch(order_of_accuracy) {
                    case(2):
                        svisc= new CDFamily(1, 2);
                        break;
                }
                break;
        }

        return svisc;
    }

    Scheme * PassViscScheme2nd() {
        int order_of_accuracy = Pass<int>(jin, "ViscOA", 2);
        Scheme* svisc= NULL;
        switch(visc_method_) {
            case(0):
                switch(order_of_accuracy) {
                    case(2):
                        svisc = new CDFamily(2, 2);
                        break;
                }
                break;
        }

        return svisc;
    }
 

    Averager * PassAverager() {
        Averager * average = NULL;
        switch(conv_method_) {
            case(0):
                average = new RoeAverager();
                break;
            default:
                break;
        }

        return average;
    }

    Transformer * PassTransformer() {
        Transformer * a = NULL;
        switch(conv_method_) {
            case(0):
                a = new Transformer();
                break;
            default:
                break;
        }

        return a;
    }

    Splitter* PassSplitter() {
        Splitter* s = NULL;
        switch(conv_method_) {
            case(0):
                s = new LFSplitter();
                break;
            default:
                break;
        }

        return s;
    }

    TimeIntegrator * PassTimeIntegrator() {
        time_method_ = Pass<int>(jin, "TimeMethod", 0);
        TimeIntegrator * a = NULL;
        switch(time_method_) {
            case(0):
                a = new LowStorageRK3();
                break;
            default:
                a = new Euler();
                break;
        }
        return a;
    }

    double Passdt() {
        double dt = Pass<double>(jin, "dt", 1.);
        return dt;
    }

    double PassCFL() {
        double CFL = Pass<double>(jin, "CFL", .1);
        return CFL;
    }

    int PassStartStep() {
        int N = Pass<int>(jin, "StartStep", 0);
        return N;
    }


    int PassRunSteps() {
        int N = Pass<int>(jin, "RunSteps", 1000);
        return N;
    }

    int PassRestartSteps() {
        int N = Pass<int>(jin, "RestartSteps", 1000);
        return N;
    }

    int PassResidualSteps() {
        int N = Pass<int>(jin, "ResidualSteps", 1000);
        return N;
    }



    int PassEvalCFLSteps() {
        int N = Pass<int>(jin, "EvalCFLSteps", 1);
        return N;
    }




};
