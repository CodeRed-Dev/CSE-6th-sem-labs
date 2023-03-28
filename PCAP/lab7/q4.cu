#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include<math.h>
__global__ void sinval(float a[], float c[],int *n)
{
    for(int i=0;i<*n;i++){
        c[i]=sin(a[i]);
    }
}
int main(void)
{
  // host copies of variables a, b & c
    int n;
    printf("Enter size: ");
    scanf("%d",&n);
    float c[n]={0};
    float a[n];
    printf("Enter array1:\n");
    for(int i=0;i<n;i++){
        scanf("%f",&a[i]);
    }
    float *d_a, *d_c;
    int *d_n; // device copies of variables a, b & c
    int size = sizeof(float)*n;
    // Allocate space for device copies of a, b, c
    cudaMalloc((void **)&d_n, sizeof(int));
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_c, size);
    // Setup input values
    
    // Copy inputs to device
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);

    cudaMemcpy(d_c, &c, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
    // Launch add() kernel on GPU
    sinval<<<1, n>>>(d_a,d_c,d_n);
    // Copy result back to host
    cudaMemcpy(&c, d_c, size, cudaMemcpyDeviceToHost);
    for(int i=0;i<n;i++){
        printf("%f ",c[i]);
    }
    // Cleanup
    cudaFree(d_a);
    cudaFree(d_n);
    cudaFree(d_c);
    return 0;
}