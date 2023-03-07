#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

void ErrorHandler(int error);
int main(int argc, char* argv[]){
    int rank, size, fact=1, factsum, i, error;
    MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    error = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0){
        ErrorHandler(error);
    }
    int sendVal = rank+1;
    error = MPI_Scan(&sendVal, &fact, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
        ErrorHandler(error);
    } 
    error = MPI_Scan(&fact, &factsum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if(rank == 0)
    {
        ErrorHandler(error);
    }
    printf("Sum of all factorial till %d! = %d\n", rank+1, factsum);

    MPI_Finalize();
    exit(0);
}

void ErrorHandler(int error){
    if(error != MPI_SUCCESS){
        char error_string[BUFSIZ];
        int length_error_string, error_class;
        MPI_Error_class(error, &error_class);
        MPI_Error_string(error, error_string, &length_error_string);
        printf("Error: %d %s\n", error_class, error_string);
    }
}
/*#include<mpi.h>
#include<stdio.h>

int main(int argc,char *argv[])
{
    int rank,size;
    int fact=1,factsum;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    for(int i=1;i<=rank;i++){
        fact=fact*i;
    }
    MPI_Scan(&fact,&factsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    printf("The value of factorial sum till Process %d is %d \n",rank,factsum);
    MPI_Finalize();
    return 0;
}*/