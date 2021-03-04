#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

vector<int> optimal_sequence(int n) {
  int maxsize = 4;
  if(n>maxsize) {
      maxsize = n+1;
  }
  vector<int> results(maxsize, 0);

  results[0] = 0;
  results[1] = 0;
  results[2] = 1;
  results[3] = 1;
  for(int i=4; i<=n; ++i) {
      int min_ops = results[i-1] + 1;
      if(i%3==0) {
          int m = results[i/3] + 1;
          if(m<min_ops) {
              min_ops = m;
          }
      } 
      if(i%2==0) {
          int m = results[i/2] + 1;
          if(m<min_ops) {
              min_ops = m;
          }
      }

      results[i] = min_ops;
  }

  std::vector<int> sequence;
  sequence.push_back(n);
  for(; n>1; ) {
      int min_ops = results[n-1];
      int last_number = n-1;
      if(n%3==0) {
          int m = results[n/3];
          if(m<min_ops) {
              min_ops = m;
              last_number = n/3;
          }
      } 
      if(n%2==0) {
          int m = results[n/2];
          if(m<min_ops) {
              min_ops = m;
              last_number = n/2;
          }
      }
      sequence.push_back(last_number);
      n = last_number;
  }

  reverse(sequence.begin(), sequence.end());
  return sequence;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> sequence = optimal_sequence(n);
  std::cout << sequence.size() - 1 << std::endl;
  for (size_t i = 0; i < sequence.size(); ++i) {
    std::cout << sequence[i] << " ";
  }
}
