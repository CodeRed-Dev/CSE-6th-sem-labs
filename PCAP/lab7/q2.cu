#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__ void add(int *a, int *b, int *c,int *n)
{
    for(int i=0;i<*n;i++){
        c[i]=a[i]+b[i];
    }
}
int main(void)
{
  // host copies of variables a, b & c
    int n;
    printf("Enter size: ");
    scanf("%d",&n);
    int c[n]={0};
    int a[n],b[n];
    printf("Enter array1:\n");
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    printf("Enter array2:\n");
    for(int i=0;i<n;i++){
        scanf("%d",&b[i]);
    }
    int *d_a, *d_b, *d_c,*d_n; // device copies of variables a, b & c
    int size = sizeof(int)*n;
    // Allocate space for device copies of a, b, c
    cudaMalloc((void **)&d_n, sizeof(int));
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);
    // Setup input values
    
    // Copy inputs to device
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);

    cudaMemcpy(d_c, &c, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);
    // Launch add() kernel on GPU
    int x=ceil(float(n)/float(256));
    printf("%d\n",x);
    add<<<x, 256>>>(d_a, d_b, d_c,d_n);
    // Copy result back to host
    cudaMemcpy(&c, d_c, size, cudaMemcpyDeviceToHost);
    for(int i=0;i<n;i++){
        printf("%d ",c[i]);
    }
    // Cleanup
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    return 0;
}