#include <iostream>
#include <vector>

using std::vector;

int get_change(int m) {
  //write your code here
  int maxsize = 5;
  if(m>maxsize) {
      maxsize = m+1;
  }
  vector<int> results(maxsize, 0);

  results[0] = 0;
  results[1] = 1;
  results[2] = 2;
  results[3] = 1;
  results[4] = 1;

  for(int i=5; i<=m; ++i) {
      vector<int> options(3, 0);
      options[0] = results[i-1] + 1;
      options[1] = results[i-3] + 1;
      options[2] = results[i-4] + 1;
      int num_coins = options[0];
      for(int j=1; j<3; ++j) {
          if(options[j]<num_coins) {
              num_coins = options[j];
          }
      }
      results[i] = num_coins;
  }

  return results[m];
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
