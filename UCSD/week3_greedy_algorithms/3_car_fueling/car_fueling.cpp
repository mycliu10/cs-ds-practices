#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

int compute_min_refills(int dist, int tank, vector<int> & stops) {
    // write your code here
    int maxsize = stops.size();
    // no stop
    if(maxsize==0) {
        if(dist<=tank) {
            return 0;
        }
        else {
            return -1;
        }
    }
    // first stop too far
    if(stops[0]>tank) {
        return -1;
    }
    // stops spaced too far
    if(maxsize>1) {
        for(int i=0; i<maxsize; ++i) {
            if(stops[i+1]-stops[i]>tank) {
                return -1;
            }
        }
    }
    // last stop too far from destination
    if(dist-stops[maxsize-1]>tank) {
        return -1;
    }

    int num_stops = 0;
    int current_stop = 0;
    int dist_travelled = 0;
    if(stops[0]==0) {
        current_stop = 1;
    }
    if(stops[maxsize-1]==dist) {
        maxsize -= 1;
    }
    for(; dist_travelled<dist; ) {
        if(dist-dist_travelled<=tank) {
            return num_stops;
        }
        int test_stop = current_stop;
        for(int i=test_stop; i<maxsize; ++i) {
            if(stops[i]-dist_travelled<=tank) {
                test_stop = i;
            }
        }
        current_stop = test_stop;
        dist_travelled = stops[current_stop];
        num_stops += 1;
    }

    return num_stops;
}


int main() {
    int d = 0;
    cin >> d;
    int m = 0;
    cin >> m;
    int n = 0;
    cin >> n;

    vector<int> stops(n);
    for (size_t i = 0; i < n; ++i)
        cin >> stops.at(i);

    cout << compute_min_refills(d, m, stops) << "\n";

    return 0;
}
