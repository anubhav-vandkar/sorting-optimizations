#include <bits/stdc++.h>
#include <time.h>
#include <chrono>

#include "merge-serial.hpp"
#include "merge-avx.hpp"
#include "merge-cuda.h"

#define MAX_LEN 100000

using namespace std;

int main() {
    vector<int> arr(MAX_LEN, 0);

    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_LEN; ++i) {
        arr[i] = (rand() % 10);
    }

    // cout << "Original array: ";
    // for (int x : arr) cout << x << " ";
    // cout << "\n";
    
    auto start_serial = chrono::high_resolution_clock::now();
    mergeSort(arr); 
    auto end_serial = chrono::high_resolution_clock::now();

    auto start_avx = chrono::high_resolution_clock::now();
    mergeSortAVX(arr); 
    auto end_avx = chrono::high_resolution_clock::now();

    auto start_cuda = chrono::high_resolution_clock::now();
    mergeSortCUDA(arr);
    auto end_cuda = chrono::high_resolution_clock::now();

    // cout << "Sorted array:   ";
    // for (int x : arr) 
    //     cout << x << " ";
    // cout << "\n";

    auto time_serial = chrono::duration_cast<std::chrono::microseconds>(end_serial-start_serial).count();
    cout<<"Time taken for serial: "<<time_serial<<" us\n";

    auto time_avx = chrono::duration_cast<std::chrono::microseconds>(end_avx-start_avx).count();
    cout<<"Time taken for avx enhanced code: "<<time_avx<<" us\n";

    auto time_cuda = chrono::duration_cast<std::chrono::microseconds>(end_cuda-start_cuda).count();
    cout<<"Time taken for avx enhanced code: "<<time_cuda<<" us\n";

    cout<<"AVX speedup: "<<time_serial/time_avx<<endl;
    
    cout<<"CUDA speedup: "<<time_serial/time_cuda<<endl;    

    return 0;
}
