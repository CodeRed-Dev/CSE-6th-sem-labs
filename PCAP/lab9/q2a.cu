#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4 //matrix size

__global__ void matrix_mult(float *a, float *b, float *c) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    for (int i = 0; i < N; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < N; ++j) {
            sum += a[row * N + j] * b[j * N + i];
        }
        c[row * N + i] = sum;
    }
}

int main() {
    int h_a[N * N] = {1, 2, 3, 4};
    int h_b[N * N] = {5, 6, 7, 8};
    int h_c[N * N];
    float *d_a, *d_b, *d_c;

/*
    // Initialize matrices
    for (int i = 0; i < N * N; ++i) {
        h_a[i] = 1.0f;
        h_b[i] = 2.0f;
    }
*/
    // Allocate memory on the GPU
    cudaMalloc(&d_a, N * N * sizeof(float));
    cudaMalloc(&d_b, N * N * sizeof(float));
    cudaMalloc(&d_c, N * N * sizeof(float));

    // Copy matrices from CPU to GPU
    cudaMemcpy(d_a, h_a, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, N * N * sizeof(float), cudaMemcpyHostToDevice);

    // Launch kernel with appropriate block and thread dimensions
    dim3 gridDim(1, 1, 1);
    dim3 blockDim(N, 1, 1);
    matrix_mult<<<gridDim, blockDim>>>(d_a, d_b, d_c);

    // Copy result from GPU to CPU
    cudaMemcpy(h_c, d_c, N * N * sizeof(float), cudaMemcpyDeviceToHost);

    // Print result
    for (int i = 0; i < N * N; ++i) {
        printf("%f ", h_c[i]);
        if ((i + 1) % N == 0) {
            printf("\n");
        }
    }

    // Free memory on GPU and CPU
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}
