#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

int get_majority_element(vector<int> &a, int left, int right) {
  //write your code here
  int maj = a[0];
  int count = 0;
  for(int i=0; i<a.size(); ++i) {
      if(count==0) {
          maj = a[i];
          ++count;
      }
      else if(maj==a[i]) {
          ++count;
      }
      else {
          --count;
      }
  }

  count = 0;
  for(int i=0; i<a.size(); ++i) {
      if(maj==a[i]) {
          ++count;
      }
  }
  if(count>a.size()/2) {
      return 0;
  }


  return -1;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
