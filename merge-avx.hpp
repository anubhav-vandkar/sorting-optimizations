#include <immintrin.h>
#include <bits/stdc++.h>

#define AVX_WINDOW 8     //256 will have 8*32 

using namespace std;

void merge_avx(vector<int>& arr, int left, int mid, int right, vector<int>& temp) {
    int i = left, j = mid, k = left;

    while (i+AVX_WINDOW <= mid && j+AVX_WINDOW <= right) {
        __m256i avx_a = _mm256_loadu_si256((__m256i const*)(arr.data() + i));
        __m256i avx_b = _mm256_loadu_si256((__m256i const*)(arr.data() + j));

        __m256i minv = _mm256_min_epi32(avx_a, avx_b);
        __m256i maxv = _mm256_max_epi32(avx_a, avx_b);

        _mm256_storeu_si256((__m256i*)(temp.data() + k), minv);
        k += AVX_WINDOW;

        if (arr[i + AVX_WINDOW - 1] <= arr[j + AVX_WINDOW - 1]) 
            i += AVX_WINDOW;
        else 
            j += AVX_WINDOW;
    }

    while (i < mid && j < right) 
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];

    while (i < mid) 
        temp[k++] = arr[i++];

    while (j < right) 
        temp[k++] = arr[j++];

    for (int t = left; t < right; ++t)
        arr[t] = temp[t];
}

void mergeSortAVX(vector<int>& arr) {
    int n = arr.size();
    vector<int> temp(n);

    for (int runSize = 1; runSize < n; runSize *= 2) {
        for (int left = 0; left < n; left += 2 * runSize) {
            int mid  = min(left + runSize, n);
            int right = min(left + 2 * runSize, n);
            merge_avx(arr, left, mid, right, temp);
        }
    }
}