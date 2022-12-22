//
// Perform "naive" square matrix multiplication
//

__global__ void matmul1(float* a, float* b, int n, float* c)
{
    // Computed subelement
    float sum = 0.0f;

    // Start indexes calculation
    int ia = (blockDim.y * blockIdx.y + threadIdx.y) * n;//ewf
    int ib = blockDim.x * blockIdx.x + threadIdx.x;
    int ic = ia + ib;

    // Multiply two matrices
    for (int k = 0; k < n; k++)
        sum += a[ia + k] * b[ib + k * n];

    // Write the block sub-matrix to global memory;
    // each thread writes one element
    c[ic] = sum;
}

#define kernel matmul1
#include "main_GPU.h"