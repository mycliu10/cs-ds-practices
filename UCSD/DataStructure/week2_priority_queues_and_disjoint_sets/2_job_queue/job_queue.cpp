#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

using std::vector;
using std::cin;
using std::cout;

class Worker {
public:
    int id_;
    long long available_time_;

    Worker(int id, long long available_time) {
        id_ = id;
        available_time_ = available_time;
//        cout << "Created id " << id_ << " with available_time = " << available_time_ << std::endl;
    }

    friend bool operator > (const Worker & a, const Worker & b) {
        if(a.available_time_ > b.available_time_) {
            return true;
        }
        else if(a.available_time_ == b.available_time_) {
            if(a.id_ > b.id_) {
                return true;
            }
        }

        return false;
    }
};


template<typename T>
void print_queue(T q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
        std::cout << q.top() << ' ';
        q.pop();
    }
    std::cout << '\n';
}


class JobQueue {
 private:
  int num_workers_;
  vector<int> jobs_;

  vector<int> assigned_workers_;
  vector<long long> start_times_;

  void WriteResponse() const {
    for (int i = 0; i < jobs_.size(); ++i) {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData() {
    int m;
    cin >> num_workers_ >> m;
    jobs_.resize(m);
    for(int i = 0; i < m; ++i)
      cin >> jobs_[i];
  }

  void AssignJobs() {
    // TODO: replace this code with a faster algorithm.
    std::priority_queue< Worker, vector<Worker>, std::greater<vector<Worker>::value_type> > pq;

    for(int i = 0; i < num_workers_; ++i) {
        Worker w(i, 0);
        pq.push(w);
    }
//    std::cout << pq.size() << std::endl;
//    while(!pq.empty()) {
//        std::cout << pq.top().id_ << ' ';
//        pq.pop();
//    }
//    std::cout << "CHECKED\n";

    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());

    for (int i = 0; i < jobs_.size(); ++i) {
        long long needed_time = jobs_[i];
        assigned_workers_[i] = pq.top().id_;
        start_times_[i] = pq.top().available_time_;
        Worker w(pq.top().id_, pq.top().available_time_+needed_time);
        pq.pop();
        pq.push(w);
    }


//    vector<long long> next_free_time(num_workers_, 0);
//    for (int i = 0; i < jobs_.size(); ++i) {
//      int duration = jobs_[i];
//      int next_worker = 0;
//      for (int j = 0; j < num_workers_; ++j) {
//        if (next_free_time[j] < next_free_time[next_worker])
//          next_worker = j;
//      }
//      assigned_workers_[i] = next_worker;
//      start_times_[i] = next_free_time[next_worker];
//      next_free_time[next_worker] += duration;
//    }
  }

 public:
  void Solve() {
    ReadData();
    AssignJobs();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  JobQueue job_queue;
  job_queue.Solve();
  return 0;
}
