#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

int optimal_weight(int W, const vector<int> &w) {
  //write your code here
  // no weights 
  int num_weights = w.size();
  if(num_weights==0) {
      return 0;
  }

  // capcity smaller than minimum weight
  int min_weight = *std::min_element(w.begin(), w.begin()+num_weights);
  if(W<min_weight) {
      return 0;
  }

  vector<int> optimal_results(W+1, 0);
  vector<int> optimal_selection((W+1)*num_weights, 0);
  for(int i=min_weight; i<=W; ++i) {
      int current_weight = 0;
      // check each weight
      for(int j=0; j<num_weights; ++j) {
          int weight = w[j];
          if(weight<=i) {
              int test_weight = 0;
              if(optimal_selection[(i-weight)*num_weights+j]==0) {
                  test_weight = optimal_results[i-weight] + weight;
              }
              else {
                  test_weight = optimal_results[i-weight];
              }

//std::cout << "Testing weight of " << weight << " at W = " << i << " with test weight = " << test_weight << std::endl;

              if(test_weight<=i and test_weight>current_weight) {
                  for(int k=0; k<num_weights; ++k) {
                      if(k!=j) {
                          optimal_selection[i*num_weights+k] = optimal_selection[(i-weight)*num_weights+k];
                      }
                  }
                  optimal_selection[i*num_weights+j] = 1;
                  current_weight = test_weight;
//std::cout << "Using ";
//for(int n=0; n<num_weights; ++n) {
//    std::cout << optimal_selection[i*num_weights+n] << " ";
//}
//std::cout << std::endl;
              }
          }
      }
      optimal_results[i] = current_weight;
//std::cout << std::endl;
  }

  return optimal_results[W];
}

int main() {
  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w) << '\n';
}
