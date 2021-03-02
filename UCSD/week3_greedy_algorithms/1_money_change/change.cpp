#include <iostream>

int get_change(int m) {
  //write your code here
  int n = 0;
  for(;m>=10;) {
      m -= 10;
      n += 1;
  }
  for(;m>=5;) {
      m -= 5;
      n += 1;
  }
  n += m;
  return n;
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
