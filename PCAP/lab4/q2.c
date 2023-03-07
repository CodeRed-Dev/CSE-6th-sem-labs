#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

void ErrorHandler(int error){
    if(error != MPI_SUCCESS){
        char error_string[BUFSIZ];
        int length_error_string, error_class;
        MPI_Error_class(error, &error_class);
        MPI_Error_string(error, error_string, &length_error_string);
        fprintf(stderr, "Error: %d %s\n", error_class, error_string);
    }
}

int main(int argc, char* argv[]){
    int rank, size, error, N;
    float result = 0.0, area = 0.0, sarea = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
    error = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    error = MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        ErrorHandler(error);
        printf("Enter the number of terms: ");
        scanf("%d",&N);
    }
    error = MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = rank; i < N; i += size){
        area = (i + 0.5)/N;
        area = (1 + area*area)*N;
        area = 4/area;
        sarea += area;
        area = 0.0;
    }

    error = MPI_Reduce(&sarea, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
        ErrorHandler(error);
        printf("The value of Pi is: %f\n", result);
    }

    MPI_Finalize();
    exit(0);
}