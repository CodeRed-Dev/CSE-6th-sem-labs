
#include<cuda.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 2

__global__ void matrixAddCol(int *a, int *b, int *c, int n)
{
    int row = blockIdx.x;
    for (int col = 0; col < n; col++)
    {
        int index = row * n + col;
        c[index] = a[index] + b[index];
    }
}

int main()
{
    int a[N*N] = {1, 2, 3, 4};
    int b[N*N] = {5, 6, 7, 8};
    int c[N*N];

    int *dev_a, *dev_b, *dev_c;

    // Allocate memory on the GPU
    cudaMalloc((void**)&dev_a, N*N*sizeof(int));
    cudaMalloc((void**)&dev_b, N*N*sizeof(int));
    cudaMalloc((void**)&dev_c, N*N*sizeof(int));

    // Copy matrices a and b from CPU to GPU
    cudaMemcpy(dev_a, a, N*N*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N*N*sizeof(int), cudaMemcpyHostToDevice);

    // Launch kernel to add matrices
    dim3 grid(N, 1, 1);
    dim3 block(1, 1, 1);
    matrixAddCol<<<grid, block>>>(dev_a, dev_b, dev_c, N);

    // Copy result matrix c from GPU to CPU
    cudaMemcpy(c, dev_c, N*N*sizeof(int), cudaMemcpyDeviceToHost);

    // Print result matrix c
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", c[i*N + j]);
        }
        printf("\n");
    }

    // Free memory on the GPU
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}

