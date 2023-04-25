#include<stdio.h>
#include "cuda.h"
#include "cuda_runtime.h"

__global__ void kcsr(int *A,int *X,int *Y,int *colindex,int *rptr,int m)
{
    int row=threadIdx.x;
    int sum;
   for(int i=0;i<m+1;i++)
   printf("insdei kernel, %d ", rptr[i]);
     if(row<m)
     {
         sum=0;
         int rstart=rptr[row];
         int rend=rptr[row+1];
         for(int i=rstart;i<rend;i++)
         sum=sum+A[i]*X[colindex[i]];
      Y[row]=Y[row]+sum;
     }
}
int main()
{
    int A[4][4]={{3,0,1,0},{0,0,0,0},{0,2,4,1},{1,0,0,1}};
    int col_index[4],csr[10];
    
    int *X,*Y;
   int M=4,N=4;
 int vsize=M*sizeof(int);
 X=(int *)malloc(vsize);
 Y=(int *)malloc(vsize);
int i,j,k=0,e=0;
 int r=0;
int rp[5],r_ptr[5];
 for(i=0;i<M;i++)
 {
     r_ptr[i]=k;
     rp[i]=r_ptr[i];
   printf("row is %d k is %d  rptr is %d ",i,k, rp[i]);
     for(j=0;j<4;j++)
     {
     if(A[i][j]!=0)
     {
         csr[k++]=A[i][j];
         col_index[e++]=j;
     }
        
     }
 
 }

printf("\n");
r_ptr[4]=k;
 //printf("\n hi e is %d, k is %d, r is %d \n",e,k,r);
for(int t=0;t<5;t++)
{
    printf("%d ",r_ptr[t]);
}

  int *d_A,*d_X,*d_Y,*rptr,*colindex;
X[0]=10;X[1]=20;X[2]=30;X[3]=40;
Y[0]=2;Y[1]=4;Y[2]=5;Y[3]=6;
 i=5;
 cudaMalloc((void **) &d_A, sizeof(int)*(k));
    cudaMemcpy(d_A, csr, sizeof(int)*(k), cudaMemcpyHostToDevice);
cudaMalloc((void **) &d_X, vsize);
    cudaMemcpy(d_X, X, vsize, cudaMemcpyHostToDevice);
cudaMalloc((void **) &d_Y, vsize);
    cudaMemcpy(d_Y, Y, vsize, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &colindex, sizeof(int)*e);
    cudaMemcpy(colindex, col_index, sizeof(int)*e, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &rptr, (sizeof(int)*i));
    cudaMemcpy(rptr, r_ptr, (sizeof(int)*i), cudaMemcpyHostToDevice);

kcsr<<<1,M>>> (d_A, d_X, d_Y, colindex, rptr,M);

 cudaMemcpy(Y, d_Y, vsize, cudaMemcpyDeviceToHost);
  for(i=0;i<M;i++)
  printf("\n%d ",Y[i]);
 return 0;
 }