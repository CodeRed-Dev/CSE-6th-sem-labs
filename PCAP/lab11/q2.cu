#include <stdio.h>
#include "cuda.h"
#include "cuda_runtime.h"
#define TW 4
#define MMW 5

__global__ void tiled1dconv(int *N, int *P, int *M, int MW, int width)
{
    __shared__ int Nds[TW + MMW - 1];
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int hli, hri;
    int n = MW / 2;
    // loading left halo elements/ghost elements
    hli = (blockIdx.x - 1) * blockDim.x + threadIdx.x;
    if (threadIdx.x >= (blockDim.x - n))
    {
        Nds[threadIdx.x - (blockDim.x - n)] = (hli < 0) ? 0 : N[hli];
    }
    // load center elements of the tiles
    Nds[n + threadIdx.x] = N[blockIdx.x * blockDim.x + threadIdx.x];
    // load right halo/ghost elements
    hri = (blockIdx.x + 1) * blockDim.x + threadIdx.x;
    if (threadIdx.x < n)
    {
        Nds[blockDim.x + n + threadIdx.x] = (hri > width) ? 0 : N[hri];
    }
    __syncthreads();
    int j, sum = 0;
    for (j = 0; j < MW; j++)
        sum = sum + M[j] * Nds[threadIdx.x + j];
    P[i] = sum;
}

int main()
{
    int A[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int Mask[5] = {1, 0, 1, 0, 1};
    int width = 16;
    int mw = 5;
    int *P;
    int *d_A, *d_m, *d_P;
    int size1 = width * (sizeof(int));
    int size2 = mw * (sizeof(int));
    P = (int *)malloc(size1);
    cudaMalloc((void **)&d_A, size1);
    cudaMemcpy(d_A, A, size1, cudaMemcpyHostToDevice);
    cudaMalloc((void **)&d_m, size2);
    cudaMemcpy(d_m, Mask, size2, cudaMemcpyHostToDevice);
    cudaMalloc((void **)&d_P, size1);
    dim3 blck(4, 1, 1);
    dim3 grid(4, 1, 1);
    tiled1dconv<<<grid, blck>>>(d_A, d_P, d_m, mw, width);
    cudaMemcpy(P, d_P, size1, cudaMemcpyDeviceToHost);
    for (int i = 0; i < width; i++)
        printf("%d  ", P[i]);
    printf("\n");
}