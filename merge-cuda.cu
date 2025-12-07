#include <cuda_runtime.h>
#include <bits/stdc++.h>

__global__ void merge_kernel(const int* d_in, int* d_out, int n, int runLen) {
    int pairIdx = blockIdx.x;
    int left = pairIdx * 2 * runLen;
    int mid  = min(left + runLen, n);
    int right = min(left + 2 * runLen, n);

    if (left >= n) return;

    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (d_in[i] <= d_in[j]) 
            d_out[k++] = d_in[i++];
        else
            d_out[k++] = d_in[j++];
    }
    while (i < mid)
        d_out[k++] = d_in[i++];

    while (j < right) 
        d_out[k++] = d_in[j++];
}

void mergeSortCUDA(std::vector<int>& arr) {
    int n = arr.size();

    if (n == 0) 
        return;

    int* d_data;
    cudaMalloc(&d_data, n*sizeof(int));
    cudaMemcpy(d_data, arr.data(), n*sizeof(int), cudaMemcpyHostToDevice);

    int* d_buf;
    cudaMalloc(&d_buf, n * sizeof(int));

    int* d_in = d_data;
    int* d_out = d_buf;

    for (int runLen = 1; runLen < n; runLen *= 2) {
        int numPairs = (n + 2 * runLen - 1) / (2 * runLen);
        merge_kernel<<<numPairs, 1>>>(d_in, d_out, n, runLen);
        cudaDeviceSynchronize();
        swap(d_in, d_out);
    }

    if (d_in != d_data) {
        cudaMemcpy(d_data, d_in, n*sizeof(int), cudaMemcpyDeviceToDevice);
    }

    cudaMemcpy(arr.data(), d_data, n*sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_data);
    cudaFree(d_buf);
}
