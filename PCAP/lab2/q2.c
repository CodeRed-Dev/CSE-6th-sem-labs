/*
#include <mpi/mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	int rank, size, num;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status st;
	if(rank == 0){
		for(int i = 1; i<size; i++){
			MPI_Send(&i, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
	else{
		MPI_Recv(&num, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &st);
		printf("In process %d, number sent is %d\n", rank, num);
	}
	MPI_Finalize();
	return 0;
}
*/
#include<stdio.h>
#include<mpi.h>
#include<string.h>
int main(int argc, char *argv[])
{
int rank,size;
//char data[100];
int num;
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Status st;
if(rank==0){
printf("Enter the number to send \n");
 scanf("%d",&num);
 for(int i = 1; i<size; i++){
			MPI_Send(&num, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
}
 else{
		MPI_Recv(&num, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &st);
		printf("Process %d recieved the number %d\n", rank, num);
	}
	MPI_Finalize();
 return 0;
}