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
long long lcm_naive(int a, int b) {


  int gcd = gcd_naive(a, b);
  return (long long) a * b / gcd;
}

int main() {
  int a, b;
  std::cin >> a >> b;
  std::cout << lcm_naive(a, b) << std::endl;
  return 0;
}
