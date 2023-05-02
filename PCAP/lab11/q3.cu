#include<stdio.h>
#include "cuda.h"
#include "cuda_runtime.h"
#define TILE_WIDTH 2 
__global__ void MatrixMulKernel(int* d_M, int* d_N, int* d_P, int Width) {
 __shared__ int Mds[TILE_WIDTH][TILE_WIDTH];
 __shared__ int Nds[TILE_WIDTH][TILE_WIDTH];
 int bx = blockIdx.x; int by = blockIdx.y;
 int tx = threadIdx.x; int ty = threadIdx.y;
// Identify the row and column of the d_P element to work on
 int Row = by * TILE_WIDTH + ty;
 int Col = bx * TILE_WIDTH + tx;
 int Pvalue = 0;
// Loop over the d_M and d_N tiles required to compute d_P element
 for (int m = 0; m < Width/TILE_WIDTH; ++m) {
// Coolaborative loading of d_M and d_N tiles into shared memory
 Mds[ty][tx] = d_M[Row*Width + m*TILE_WIDTH + tx];
 Nds[ty][tx] = d_N[(m*TILE_WIDTH + ty)*Width + Col];
 __syncthreads();
 for (int k = 0; k < TILE_WIDTH; ++k) {
 Pvalue += Mds[ty][k] * Nds[k][tx];
}
 __syncthreads();
}
 d_P[Row*Width + Col] = Pvalue;
}
void vecAdd(int* A, int *B, int* C,int m,int n,int p,int q)
{    int size1 = (m*n) * sizeof(int);
 int size2 = (p*q) * sizeof(int);
  int size3 = (m*q) * sizeof(int);
    //printf("elements m,n are %d%d%d",elements,m,n);
     int *d_A, *d_B, *d_C;
    cudaMalloc((void **) &d_A, size1);
    cudaMemcpy(d_A, A, size1, cudaMemcpyHostToDevice);
    cudaMalloc((void **) &d_B, size2);
    cudaMemcpy(d_B, B, size2, cudaMemcpyHostToDevice);
    cudaMalloc((void **) &d_C, size3);
      // Kernel invocation code
 dim3 grid(ceil(n/((TILE_WIDTH)*1.0)),ceil(n/((TILE_WIDTH)*1.0)),1);
 dim3 blck(TILE_WIDTH,TILE_WIDTH,1);
    MatrixMulKernel<<<grid,blck>>> (d_A, d_B,d_C,n);
     cudaMemcpy(C, d_C, size3,cudaMemcpyDeviceToHost);
     // Free device memory for A, B, C
    cudaFree(d_A); cudaFree (d_C);
}
int main()
{
// Host data buffers
int *A = NULL;       // Input array
int *B = NULL;       // Input array
int *C = NULL;       // Output array
// Elements in each array
 int n=4,m=4;
 int p=4,q=4;
const int elements = m*n; // size of host data buffers
// Compute the size of the data in bytes
const int elements1 = p*q;
size_t datasize = sizeof(int)*elements;
size_t datasize1 = sizeof(int)*elements1;
// Dynamically allocate space for input/output host data buffers
A = (int*)malloc(datasize);
B = (int*)malloc(datasize1);
C = (int*)malloc(sizeof(int)*(m*q));
 int D[m][q];
// Initialize the input data
for(int i = 0; i < elements; i++)
{
   A[i] = i+1;
}
 for(int i = 0; i < elements1; i++)
{
   B[i] = i+1;
}
 for(int i = 0; i < elements; i++)
{ 
    printf("%d ",A[i]);
}
 for(int i = 0; i < elements1; i++)
{ 
    printf("%d ",B[i]);
}
 printf("\n");
vecAdd( A,B, C,m,n,p,q);
// Verify the output
bool result = true;
 int j,k=0;
for(int i = 0; i < m; i++)
{ 
    for(j=0;j<q;j++)
    D[i][j]=C[k++];
}
 for(int i = 0; i < m; i++)
{
    for(j=0;j<q;j++)
    printf("%d ",D[i][j]);
    printf("\n");
}
if(result)
        printf("Output is correct\n");
 else
     printf("Output is incorrect\n");
free(A); free(B); free(C);
return 0;
}