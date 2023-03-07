#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[])
{
	int rank;
	int N;
	int size;
	int A[100];
	int B[100];
	int c[100];
	int i;
	int M;
	int average;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
	{
		N=size;
		printf("Enter M ki value\n");
		scanf("%d",&M);
		printf("Enter %d value\n",M*N);
		fflush(stdout);
		for(int i=0;i<M*N;i++){
			scanf("%d",&A[i]);
		}
	}
	MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(A,M,MPI_INT,c,M,MPI_INT,0,MPI_COMM_WORLD);
	printf("I have received following in process %d\n",rank);
	float sum;
	sum=0;
	for(int i=0;i<M;i++)
	{
		printf("%d ",c[i]);
		sum=sum+c[i];
	}
	printf("\n");
	average=sum/M;
	printf("average is %d\n",average);
	MPI_Gather(&average,1,MPI_INT,B,1,MPI_INT,0,MPI_COMM_WORLD);
	if(rank==0)
	{
		printf("The result of this gathered in the root is:-\n");
		int sum;
		sum=0;
		for(i=0;i<N;i++)
		{
			sum=sum+B[i];
			printf("%d\t",B[i]);
		}
		printf("Total average is %d\n",sum/N);
	}

	MPI_Finalize();
}