

#include <stdio.h>

#include "cuda.h"

#include "cuda_runtime.h"

__host__ __device__ int strlen1(char *A)

{

    int i, c = 0;

    for (i = 0; A[i] != '\0'; i = i + 1)

    {

        c = c + 1;
    }

    return c;
}

__global__ void vecConv(float *A, float *B, float *C, int n, int l1, int kl, int p)

{

    int i = (threadIdx.x + blockDim.x * blockIdx.x);

    float sum = 0;

    int j = 0, c, k, l;

    c = kl / 2;

    if (i >= p && i < n - p)

    {

        sum = sum + A[i] * B[c];

        for (l = 0, j = c - 1, k = i - 1; l < c; j--, k--, l++)

            sum = sum + A[k] * B[j];

        for (l = 0, j = c + 1, k = i + 1; l < c && j < kl; j++, k++, l++)

            sum = sum + A[k] * B[j];
    }

    C[i - p] = sum;
}

void vecAdd(float *A, float *B, float *C, int n, int l1, int kl, int p)

{
    int size = n * sizeof(float);

    int size1 = kl * sizeof(float);

    int size2 = l1 * sizeof(float);

    float *d_A;

    float *d_B, *d_C;

    cudaMalloc((void **)&d_A, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);

    cudaMalloc((void **)&d_B, size1);

    cudaMemcpy(d_B, B, size1, cudaMemcpyHostToDevice);

    cudaMalloc((void **)&d_C, size2);

    // Kernel invocation code

    vecConv<<<1, 32>>>(d_A, d_B, d_C, n, l1, kl, p);

    cudaMemcpy(C, d_C, size2, cudaMemcpyDeviceToHost);

    // Free device memory for A, B, C

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

int main()

{

    // Host data buffers

    int p, d = 1, s = 1;

    int l1, kl, i;

    l1 = 7;

    kl = 3;

    p = (s * (l1 - 1) - l1 + d * (kl - 1) + 1) / 2;

    printf(" p is %d", p);

    float *A = NULL; // Input array

    float *B = NULL; // Input array

    float *C = NULL; // Output array

    // Elements in each array

    const int elements = l1 + 2 * p; // size of host data buffers

    // Compute the size of the data in bytes

    printf(" ele is %d", elements);

    size_t datasize = sizeof(float) * elements;

    size_t datasize1 = sizeof(float) * kl;

    size_t datasize2 = sizeof(float) * l1;

    // Dynamically allocate space for input/output host data buffers

    A = (float *)malloc(datasize);

    B = (float *)malloc(datasize1);

    C = (float *)malloc(datasize2);

    // Initialize the input data

    for (i = 0; i < p; i++)

    {

        A[i] = 0;

        A[elements - i - 1] = 0;
    }

    int k = p;

    A[k] = 10;
    k = k + 1;

    A[k] = 50;
    k = k + 1;

    A[k] = 60;
    k = k + 1;

    A[k] = 10;
    k = k + 1;

    A[k] = 20;
    k = k + 1;

    A[k] = 40;
    k = k + 1;

    A[k] = 30;
    k = k + 1;

    // A[k]=10;k=k+1;

    // A[k]=5;k=k+1;

    // A[k]=9;k=k+1;

    // A[k]=8;k=k+1;

    // A[k]=4;k=k+1;

    // A[k]=3;k=k+1;

    for (i = 0; i < elements; i++)

    {

        printf("%f ", A[i]);
    }

    printf("\n");

    B[0] = 0.333;
    B[1] = 0.333;
    B[2] = 0.333;
    B[3] = 0.333; // B[4]=5;

    for (i = 0; i < kl; i++)

    {

        printf("%f ", B[i]);
    }

    vecAdd(A, B, C, elements, l1, kl, p);

    // Verify the output

    bool result = true;

    printf("\n array is ");

    for (int i = 0; i < l1; i++)

    {

        printf(" %f ", C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}