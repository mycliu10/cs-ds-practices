#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using std::vector;

int partition3(vector<int> &A) {
  //write your code here
  if(A.size() == 0) {
      return 1;
  }

  int sumA = std::accumulate(A.begin(), A.end(), 0);
  if(sumA%3 > 0) {
      return 0;
  }

  int maxA = *std::max_element(A.begin(), A.end());
  int aim = sumA / 3;
  if(maxA > aim) {
      return 0;
  }

  int maxsize = A.size() + 1; 
  int aimsize = aim + 1;
  vector<int> results(maxsize*aimsize*aimsize*aimsize, 0);

  // no element 0 aim
  for(int n=0; n<=0; ++n) {
      for(int i=0; i<=0; ++i) {
          for(int j=0; j<=0; ++j) {
              for(int k=0; k<=0; ++k) {
                  int index = ((n * aimsize + i) * aimsize + j) * aimsize + k;
                  results[index] = 1;
              }
          }
      }
  }



  for(int n=1; n<=A.size(); ++n) {
      for(int i=0; i<=aim; ++i) {
          for(int j=0; j<=aim; ++j) {
              for(int k=0; k<=aim; ++k) {
                  int is_doable = 0;
                  int item = A[n-1];
                  if(item <= i) {
                      int index = (((n-1) * aimsize + i-item) * aimsize + j) * aimsize + k;
                      is_doable = results[index];
                  }
                  if(is_doable==0) {
                      if(item <= j) {
                          int index = (((n-1) * aimsize + i) * aimsize + j-item) * aimsize + k;
                          is_doable = results[index];
                      }
                  }
                  if(is_doable==0) {
                      if(item <= k) {
                          int index = (((n-1) * aimsize + i) * aimsize + j) * aimsize + k-item;
                          is_doable = results[index];
                      }
                  }

                  int index = ((n * aimsize + i) * aimsize + j) * aimsize + k;
                  results[index] = is_doable;
//std::cout << item << " elements with aims of " << i << " " << j << " " << k << " is " << is_doable << std::endl;
              }
          }
      }
  }

  return results[((A.size() * aimsize + aim) * aimsize + aim) * aimsize + aim];




  return 0;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> A(n);
  for (size_t i = 0; i < A.size(); ++i) {
    std::cin >> A[i];
  }
  std::cout << partition3(A) << '\n';
}
