#include <iostream>

int gcd_naive(int a, int b) {
  for(; a>0 and b>0; ) {
      if(a>b) {
          a = a%b;
      }
      else {
          b = b%a;
      }
  }
  if(a>0) {
      return a;
  }
  else {
      return b;
  }
}

int main() {
  int a, b;
  std::cin >> a >> b;
  std::cout << gcd_naive(a, b) << std::endl;
  return 0;
}
