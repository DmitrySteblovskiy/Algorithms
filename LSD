#include <iostream>
#include <cmath>

int digit(unsigned long long n, unsigned long long k) {
    std::cout << n << " " << k;
    return n & k == k ? 1 : 0;
}

void radixSort(unsigned long long *a, int n, int m){
    int* c = new int[2];
    unsigned long long* b = new unsigned long long[n];
    int bin = 1;
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            c[j] = 0;
        }
        for (size_t j = 0; j < n; ++j) {
            ++c[digit(a[j], bin)];
        }
        std::cout << c[0] << " " << c[1] << std::endl;
        int count = 0;
        
        for (size_t j = 0; j < 2; ++j) {
            int t = c[j];
            c[j] = count;
            count += t;
        }
        for (size_t j = 0; j < n; ++j) {
            b[c[digit(a[j], bin)]++] = a[j];
        }
        for (size_t j = 0; j < n; ++j) {
            a[j] = b[j];
            std::cout << a[j] << " ";
        }
        std::cout << std::endl;
        bin *= 2;
        std::cout << bin << std::endl;
    }
    delete []b;
    delete []c;
}


int main() {
    int n;
    std::cin >> n;
    unsigned long long* a = new unsigned long long[n];
    
    for (size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    radixSort(a, n, 32);
    for (int i = 0; i < n; ++i)
        std::cout << a[i] << " ";
        
}
