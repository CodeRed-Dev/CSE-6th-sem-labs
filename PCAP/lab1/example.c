// first MPI inter process communication program
// 31 Jan 2023
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init (&argc, &argv);  //initialize MPI library
	MPI_Comm_size(MPI_COMM_WORLD, &size); // get number of process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get rank of the process

	printf("Processor %d of %d :hello world\n",rank, size);
	MPI_Finalize(); //MPI cleanup
	return 0;
}

