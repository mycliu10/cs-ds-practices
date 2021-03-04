#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

int edit_distance_recursive(const string &str1, const string &str2, const int n1, const int n2) {
    if(n1==0) {
        return n2;
    }
    if(n2==0) {
        return n1;
    }

    int result = 0;
    if(str1[n1-1]==str2[n2-1]) {
        result = edit_distance_recursive(str1, str2, n1-1, n2-1);
    }
    else {
        int option1 = edit_distance_recursive(str1, str2, n1-1, n2)   + 1;
        int option2 = edit_distance_recursive(str1, str2, n1,   n2-1) + 1;
        int option3 = edit_distance_recursive(str1, str2, n1-1, n2-1) + 1;
        result = std::min(std::min(option1,option2), option3);
    }

    return result;
}

int edit_distance(const string &str1, const string &str2) {
  //write your code here
  int n1 = str1.size();
  int n2 = str2.size();
  if(n1==0) {
      return n2;
    }
  if(n2==0) {
      return n1;
  }

  int maxsize = (n1+1)*(n2+1);
  std::vector<int> results(maxsize, 0);

  for(int i=0,j=0; i<=n1; ++i) {
      results[i*(n2+1)+j] = i;
  }
  for(int j=0,i=0; j<=n2; ++j) {
      results[i*(n2+1)+j] = j;
  }
  for(int i=1; i<=n1; ++i) {
      for(int j=1; j<=n2; ++j) {
          int option1 = 0;
          if(str1[i-1]==str2[j-1]) {
              option1 = results[(i-1)*(n2+1)+j-1];
          } 
          else {
              option1 = results[(i-1)*(n2+1)+j-1] + 1;
          }
          int option2 = results[(i-1)*(n2+1)+j  ] + 1;
          int option3 = results[(i  )*(n2+1)+j-1] + 1;
          results[i*(n2+1)+j] = std::min(std::min(option1,option2), option3);
      }
  }

  return results[n1*(n2+1)+n2];


//  return edit_distance_recursive(str1, str2, str1.size(), str2.size());
}

int main() {
  string str1;
  string str2;
  std::cin >> str1 >> str2;
  std::cout << edit_distance(str1, str2) << std::endl;
  return 0;
}
