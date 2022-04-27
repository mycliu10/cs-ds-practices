#pragma once

class Timer {
public:
    double dt_;
    double time_;
    
    Timer();

    double SetProbe(const int tag);
    double GetProbe(const int tag);

private:
    double time_probes_[2][16];

};
