#include <bits/stdc++.h>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right, vector<int>& temp) {
    int i = left, j = mid, k = left;

    while (i < mid && j < right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i < mid) 
        temp[k++] = arr[i++];

    while (j < right) 
        temp[k++] = arr[j++];

    for (int t = left; t < right; ++t) 
        arr[t] = temp[t];
}

void mergeSort(vector<int>& arr) {
    int n = arr.size();
    vector<int> temp(n);

    for (int runSize = 1; runSize < n; runSize *= 2) {
        for (int left = 0; left < n; left += 2 * runSize) {
            int mid  = min(left + runSize, n);
            int right = min(left + 2 * runSize, n);
            merge(arr, left, mid, right, temp);
        }
    }
}