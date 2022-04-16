#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <mpi.h>


#include <iostream>
using std::cout;
using std::endl;



static const int PROFILER_BUFFER_SIZE = 128;

class Profiler {
private:
    int is_enabled_;
    double timers_[PROFILER_BUFFER_SIZE*2];
    double times_ [PROFILER_BUFFER_SIZE];
    int    counts_[PROFILER_BUFFER_SIZE];
    int    tags_  [PROFILER_BUFFER_SIZE];
    int num_tags_;
    std::string names_[PROFILER_BUFFER_SIZE];
public:
    Profiler() : is_enabled_{1}, times_{0.}, num_tags_{0} {} 

    int Register(const int tag, const std::string name) {
        times_[tag] = 0.;
        names_[tag] = name;
        counts_[tag] = 0;
        tags_[num_tags_++] = tag;

        return 0;
    }

    int Enable() {
        is_enabled_ = 1;
        return 0;
    }

    int Disenable() {
        is_enabled_ = 0;
        return 0;
    }


    int Start(const int tag) {
        if(is_enabled_ == 0) {
            return 1;
        }
        timers_[2*tag] = MPI_Wtime();
        return 0;
    }
    
    int Pause(const int tag) {
        if(is_enabled_ == 0) {
            return 1;
        }
        timers_[2*tag+1] = MPI_Wtime();
        double time = timers_[2*tag+1] - timers_[2*tag];
        times_[tag] += time/60.;
        counts_[tag] += 1;
        return 0;
    }

    double GetTime(const int tag) {
        return times_[tag];
    }

    std::string GetName(const int tag) {
        return names_[tag];
    }

    int Report(int tag) {
        cout << names_[tag] << std::right << std::setw(10) << "    " << times_[tag] << " repeated " << counts_[tag] << " times." << endl;

        return 0;
    }

    int Report() {
        for(int i=0; i<num_tags_; ++i) {
            int tag = tags_[i];
            Report(tag);
        }

        return 0;
    }
 
 
};

extern Profiler global_profiler;
