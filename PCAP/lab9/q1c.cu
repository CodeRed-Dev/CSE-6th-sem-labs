#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2

__global__ void matrixAddElement(int *a, int *b, int *c, int num_rows, int num_cols)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid < num_rows * num_cols)
    {
        int i = tid / num_cols;
        int j = tid % num_cols;
        c[tid] = a[tid] + b[tid];
    }
}

int main()
{
    int a[N * N] = {1, 2, 3, 4};
    int b[N * N] = {5, 6, 7, 8};
    int c[N * N];

    int *dev_a, *dev_b, *dev_c;

    // Allocate memory on the GPU
    cudaMalloc((void **)&dev_a, N * N * sizeof(int));
    cudaMalloc((void **)&dev_b, N * N * sizeof(int));
    cudaMalloc((void **)&dev_c, N * N * sizeof(int));

    // Copy matrices a and b from CPU to GPU
    cudaMemcpy(dev_a, a, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // Launch kernel to add matrices
    dim3 grid(1 + ((N * N - 1) / 256), 1, 1);
    dim3 block(256, 1, 1);
    matrixAddElement<<<grid, block>>>(dev_a, dev_b, dev_c, N, N);

    // Copy result matrix c from GPU to CPU
    cudaMemcpy(c, dev_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    // Print result matrix c

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", c[i * N + j]);
        }
        printf("\n");
    }

    // Free memory on the GPU
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}
