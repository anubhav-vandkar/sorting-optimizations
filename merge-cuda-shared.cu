#include <cuda_runtime.h>
#include <vector>

__device__ __forceinline__ int merge_path_search(const int* A, int alen, const int* B, int blen, int diag) {
    int lo = max(0, diag - blen);
    int hi = min(diag, alen);
    while (lo < hi) {
        int i = (lo + hi) >> 1;
        int j = diag - i;

        int Ai   = (i < alen) ? A[i] : INT_MAX;
        int Aim1 = (i > 0)    ? A[i - 1] : INT_MIN;
        int Bj   = (j < blen) ? B[j] : INT_MAX;
        int Bjm1 = (j > 0)    ? B[j - 1] : INT_MIN;

        if (Aim1 > Bj) {
            hi = i;
        } else if (Ai < Bjm1) {
            lo = i + 1;
        } else {
            return i;
        }
    }
    return lo;
}

__global__ void merge_kernel_parallel(const int* __restrict__ d_in, int* __restrict__ d_out, int n, int runLen)
{
    int pairIdx = blockIdx.x;
    int left  = pairIdx * 2 * runLen;
    int mid   = min(left + runLen, n);
    int right = min(left + 2 * runLen, n);

    if (left >= n) return;

    const int* A = d_in + left;
    const int* B = d_in + mid;
    int alen = mid - left;
    int blen = right - mid;

    int total = alen + blen;
    int t = threadIdx.x;
    int stride = blockDim.x;

    for (int outIdx = t; outIdx < total; outIdx += stride) {
        int i = merge_path_search(A, alen, B, blen, outIdx);
        int j = outIdx - i;

        int Ai = (i < alen) ? A[i] : INT_MAX;
        int Bj = (j < blen) ? B[j] : INT_MAX;

        d_out[left + outIdx] = (Ai <= Bj) ? Ai : Bj;
    }
}

void mergeSortCUDA_Shared(std::vector<int>& arr) {
    int n = (int)arr.size();
    if (n == 0) return;

    int* d_data = nullptr;
    int* d_buf  = nullptr;
    cudaMalloc(&d_data, n * sizeof(int));
    cudaMalloc(&d_buf,  n * sizeof(int));
    cudaMemcpy(d_data, arr.data(), n * sizeof(int), cudaMemcpyHostToDevice);

    int* d_in  = d_data;
    int* d_out = d_buf;

    const int threadsPerBlock = 256;

    for (int runLen = 1; runLen < n; runLen <<= 1) {
        int numPairs = (n + 2 * runLen - 1) / (2 * runLen);

        merge_kernel_parallel<<<numPairs, threadsPerBlock>>>(d_in, d_out, n, runLen);

        std::swap(d_in, d_out);
    }

    if (d_in != d_data) {
        cudaMemcpy(d_data, d_in, n * sizeof(int), cudaMemcpyDeviceToDevice);
    }
    cudaMemcpy(arr.data(), d_data, n * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_data);
    cudaFree(d_buf);
}