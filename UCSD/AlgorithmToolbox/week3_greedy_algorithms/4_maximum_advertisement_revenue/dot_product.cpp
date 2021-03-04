#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

long long max_dot_product(vector<int> a, vector<int> b) {
  // write your code here
  int maxsize = a.size();

  long long result = 0;
  for(int n=0; n<maxsize; ++n) {
      int index_max_a = 0;
      int max_a = a[0];
      for(int i=1; i<maxsize-n; ++i) {
          if(a[i]>max_a) {
              max_a = a[i];
              index_max_a = i;
          }
      }
      int index_max_b = 0;
      int max_b = b[0];
      for(int i=1; i<maxsize-n; ++i) {
          if(b[i]>max_b) {
              max_b = b[i];
              index_max_b = i;
          }
      }
      result += ((long long) max_a) * max_b;
      a.erase(a.begin()+index_max_a);
      b.erase(b.begin()+index_max_b);
  }

  return result;



  for (size_t i = 0; i < a.size(); i++) {
    result += ((long long) a[i]) * b[i];
  }
  return result;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> a(n), b(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  for (size_t i = 0; i < n; i++) {
    std::cin >> b[i];
  }
  std::cout << max_dot_product(a, b) << std::endl;
}
