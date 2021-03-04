#include <iostream>
#include <vector>
#include <algorithm>

long long MaxPairwiseProduct(const std::vector<int>& numbers) {
    long long max_product = 0;
    int n = numbers.size();

    if(n == 2) {
        return (long long) numbers[0] * numbers[1];
    }

    int index_largest = 0; 
    int largest = numbers[0];
    for(int i = 1; i<n; ++i) {
        if(numbers[i] > largest) {
            largest = numbers[i];
            index_largest = i;
        }
    }

    int index_second_largest = 0; 
    int second_largest = numbers[0];
    if(index_largest == 0) {
        int index_second_largest = 1; 
        int second_largest = numbers[1];
    }

    for(int i = 1; i<n; ++i) {
        if(numbers[i] > second_largest and i != index_largest) {
            second_largest = numbers[i];
            index_second_largest= i;
        }
    }
    
    max_product = (long long) numbers[index_largest] * numbers[index_second_largest];

    return max_product;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << MaxPairwiseProduct(numbers) << "\n";
    return 0;
}
