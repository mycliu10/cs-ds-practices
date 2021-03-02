#include <iostream>
#include <vector>

using std::vector;

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
  double value = 0.0;

  // write your code here
  int size = weights.size();
  vector<double> value_per_weight(size, 0.);
  for(int i=0; i<size; ++i) {
      value_per_weight[i] = double(values[i]) / double(weights[i]);
  }

  for(;capacity>0;) {
      double max_value_per_weight = value_per_weight[0];
      int index = 0;
      for(int i=1; i<size; ++i) {
          if(value_per_weight[i]>max_value_per_weight) {
              max_value_per_weight = value_per_weight[i];
              index = i;
          }
      }
      if(capacity>=weights[index]) {
          value += double(weights[index]) * max_value_per_weight;
          capacity -= weights[index];
          value_per_weight[index] = 0.;

      } 
      else {
          value += double(capacity) * max_value_per_weight;
          capacity = 0;
      }
  }


  return value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values);

  std::cout.precision(10);
  std::cout << optimal_value << std::endl;
  return 0;
}
