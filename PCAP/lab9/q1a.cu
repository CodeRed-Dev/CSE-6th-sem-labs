
#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2

__global__ void add_matrices_row(float *a, float *b, float *c, int num_rows, int num_cols) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid < num_rows) {
        for (int j = 0; j < num_cols; j++) {
            c[tid*num_cols+j] = a[tid*num_cols+j] + b[tid*num_cols+j];
        }
    }
}

int main()
{
    float a[N * N] = {1.0, 2.0, 3.0, 4.0};
    float b[N * N] = {5.0, 6.0, 7.0, 8.0};
    float c[N * N];

    float *dev_a, *dev_b, *dev_c;

    // Allocate memory on the GPU
    cudaMalloc((void **)&dev_a, N * N * sizeof(float));
    cudaMalloc((void **)&dev_b, N * N * sizeof(float));
    cudaMalloc((void **)&dev_c, N * N * sizeof(float));

    // Copy matrices a and b from CPU to GPU
    cudaMemcpy(dev_a, a, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N * N * sizeof(float), cudaMemcpyHostToDevice);

    // Launch kernel to add matrices
    dim3 grid(1 + ((N - 1) / 256), 1, 1);
    dim3 block(256, 1, 1);
    add_matrices_row<<<grid, block>>>(dev_a, dev_b, dev_c, N, N);

    // Copy result matrix c from GPU to CPU
    cudaMemcpy(c, dev_c, N * N * sizeof(float), cudaMemcpyDeviceToHost);

    // Print result matrix c
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%f ", c[i * N + j]);
        }
        printf("\n");
    }

    // Free memory on the GPU
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}
