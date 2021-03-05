#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
 private:
  vector<int> data_;
  vector< pair<int, int> > swaps_;

  void WriteResponse() const {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < swaps_.size(); ++i) {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
//cout << std::endl;
//for(int i = 0; i < data_.size(); ++i) {
//    cout << data_[i] << " ";
//}
//cout << std::endl;
  }

  void ReadData() {
    int n;
    cin >> n;
    data_.resize(n);
    for(int i = 0; i < n; ++i)
      cin >> data_[i];
  }

  void GenerateSwaps() {
    swaps_.clear();
    // The following naive implementation just sorts 
    // the given sequence using selection sort algorithm
    // and saves the resulting sequence of swaps.
    // This turns the given array into a heap, 
    // but in the worst case gives a quadratic number of swaps.
    //
    // TODO: replace by a more efficient implementation

    // binary min-heap
    for( ; ; ) {
        int count = 0;
        int n = data_.size();
        for(int i = n-1; i >= 1; --i) {
            int index_self = i;
            for( ; ; ) {
                int index_parent = (index_self+1)/2 - 1;
                if(data_[index_self] < data_[index_parent]) {
                    swap(data_[index_self], data_[index_parent]);
                    swaps_.push_back(make_pair(index_parent, index_self));
                    index_self = index_parent;
                    ++count;
                    if(index_self == 0) {
                        break;
                    }
                }
                else {
                    break;
                }
            }
        }
        if(count==0) {
            break;
        }
    }
// selection sort
//    for (int i = 0; i < data_.size(); ++i)
//      for (int j = i + 1; j < data_.size(); ++j) {
//        if (data_[i] > data_[j]) {
//          swap(data_[i], data_[j]);
//          swaps_.push_back(make_pair(i, j));
//        }
//      }
//    }
  }

 public:
  void Solve() {
    ReadData();
    GenerateSwaps();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
