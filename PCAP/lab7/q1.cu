#include<stdio.h>
#include"cuda_runtime.h"
#include"device_launch_parameters.h"

//using BLOCK of size const size 10

const short N = 10;

__global__ void vecadd(int *d_a, int *d_b,int *d_c)
{
	unsigned short tid = blockIdx.x;
	
	if(tid < N)
	{
		d_c[tid] = d_a[tid] + d_b[tid];
	}
}

int main (void)
{
	int Host_a[N], Host_b[N], Host_c[N];

	int *d_a , *d_b, *d_c ;

      
      cudaMalloc((void **)&d_a , N*sizeof(int));
      cudaMalloc((void **)&d_b , N*sizeof(int));
      cudaMalloc((void **)&d_c , N*sizeof(int));

      //random values in the host array on cpu
      for ( int i = 0; i <N ; i++ )
      {
            Host_a[i] = i ;
            Host_b[i] = 2*i ; 
      }

      
      cudaMemcpy (d_a , Host_a , N*sizeof(int) , cudaMemcpyHostToDevice);
      cudaMemcpy (d_b , Host_b , N*sizeof(int) , cudaMemcpyHostToDevice);
      //just switch places of 1 and N for the execution of N threads
      //curently for block size as N
      vecadd<<<N,1>>> (d_a, d_b , d_c ) ;
      cudaMemcpy(Host_c , d_c , N*sizeof(int) , cudaMemcpyDeviceToHost);

      for ( int i = 0; i<N; i++ )
                  printf ("%d + %d = %d\n", Host_a[i] , Host_b[i] , Host_c[i] ) ;

      cudaFree (d_a) ;
      cudaFree (d_b) ;
      cudaFree (d_c) ;
      return 0 ;

}
