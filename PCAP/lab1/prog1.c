#include<mpi.h>
#include<stdio.h>

void power(int const x,int id)
{
	int r =1,i;
	for(i=0;i<id;i++)
	r = r*x;
	printf("the pow(%d,%d) is %d\n", x, id, r);
}		

int main(int argc, char *argv[])
{
	int id_rank, nop;
	int const x = 2;
	MPI_Init (&argc, &argv);  //initialize MPI library
	MPI_Comm_size(MPI_COMM_WORLD, &nop); // get number of process
	MPI_Comm_rank(MPI_COMM_WORLD, &id_rank);
	power(x, id_rank);
	MPI_Finalize();
	
	return 0;
}