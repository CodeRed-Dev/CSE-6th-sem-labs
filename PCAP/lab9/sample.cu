/*
#include <stdio.h>

#define N 2

__global__ void matrixAdd(int (*a)[N], int (*b)[N], int (*c)[N])
{
    int col = blockIdx.x;
    for (int row = 0; row < N; row++)
    {
        c[row][col] = a[row][col] + b[row][col];
    }
}

int main()
{
    int a[N][N] = {{1, 2}, {3, 4}};
    int b[N][N] = {{5, 6}, {7, 8}};
    int c[N][N];

    int (*dev_a)[N], (*dev_b)[N], (*dev_c)[N];

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
    matrixAdd<<<grid, block>>>(dev_a, dev_b, dev_c);

    // Copy result matrix c from GPU to CPU
    cudaMemcpy(c, dev_c, N*N*sizeof(int), cudaMemcpyDeviceToHost);

    // Print result matrix c
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    // Free memory on the GPU
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 256

__global__ void add_matrices_row(float *a, float *b, float *c, int num_rows, int num_cols) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid < num_rows) {
        for (int j = 0; j < num_cols; j++) {
            c[tid*num_cols+j] = a[tid*num_cols+j] + b[tid*num_cols+j];
        }
    }
}

int main() {
    int num_rows, num_cols, size;
    float *h_a, *h_b, *h_c;
    float *d_a, *d_b, *d_c;

    // Get matrix dimensions from user
    printf("Enter number of rows: ");
    scanf("%d", &num_rows);
    printf("Enter number of columns: ");
    scanf("%d", &num_cols);

    // Allocate memory for matrices on host
    size = num_rows * num_cols * sizeof(float);
    h_a = (float*) malloc(size);
    h_b = (float*) malloc(size);
    h_c = (float*) malloc(size);

    // Get matrix elements from user
    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            scanf("%f", &h_a[i*num_cols+j]);
        }
    }

    printf("Enter elements of matrix B:\n");
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            scanf("%f", &h_b[i*num_cols+j]);
        }
    }

    // Allocate memory for matrices on device
    cudaMalloc((void**) &d_a, size);
    cudaMalloc((void**) &d_b, size);
    cudaMalloc((void**) &d_c, size);

    // Copy matrices from host to device
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // Add matrices using one thread per row
    add_matrices_row<<<(num_rows+BLOCK_SIZE-1)/BLOCK_SIZE, BLOCK_SIZE>>>(d_a, d_b, d_c, num_rows, num_cols);

    // Copy result matrix from device to host
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    // Print result matrix
    printf("Result matrix using one thread per row:\n");
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++) {
            printf("%.2f ", h_c[i*num_cols+j]);
        }
        printf("\n");
    }

    // Free memory on host and device
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
