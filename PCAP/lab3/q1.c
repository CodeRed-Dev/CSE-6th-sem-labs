#include <mpi.h>
#include <stdio.h>

int fact(int x)
{
	if(x==0)
	{
		return 1;
	}
	else
	{
		return x*fact(x-1);
	}
}

int main(int argc, char *argv[])
{
	int rank, size, N, A[10], B[10], c, i;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0)
	{
		N=size;
		printf("Enter %d values: \n",N);
		for(i=0;i<N;i++)
			scanf("%d",&A[i]);
	}

	MPI_Scatter(A,1,MPI_INT,&c,1,MPI_INT,0,MPI_COMM_WORLD);
	printf("I have recieved %d in process %d \n",c,rank);
	fflush(stdout);

	c=fact(c);
	MPI_Gather(&c,1,MPI_INT,B,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank == 0)
	{
		printf("The Results gathered in the root\n");
		int sum;
			sum=0;
		for(i=0;i<N;i++){
			sum=sum+B[i];
			printf("%d\t",B[i]);
		}
			printf("\nTotal sum is %d\n",sum);
	}
	MPI_Finalize();
	return 0;

}

