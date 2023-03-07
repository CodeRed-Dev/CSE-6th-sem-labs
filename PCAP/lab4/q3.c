#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

void ErrorHandler(int error){
    if(error != MPI_SUCCESS){
        char error_string[BUFSIZ];
        int length_err_string, error_class;
        MPI_Error_class(error, &error_class);
        MPI_Error_string(error, error_string, &length_err_string);
        printf("Error: %d %s\n", error_class, error_string);
    }
}

int main(int argc, char* argv[]){
    int rank, size, error, ele, result;
    int mat[3][3];
    MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
    error = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    error = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0)
        ErrorHandler(error);

    if(rank == 0){
        printf("Enter the elements in 3x3 matrix:\n");
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                scanf("%d",&mat[i][j]);
            }
        }
        printf("Enter element to be searched: ");
        scanf("%d",&ele);
    }
    int sMat[3];
    error = MPI_Bcast(&ele, 1, MPI_INT, 0, MPI_COMM_WORLD);
    error = MPI_Scatter(mat, 3, MPI_INT, sMat, 3, MPI_INT, 0, MPI_COMM_WORLD);
    int res = 0;
    for(int i=0;i<3;i++){
        if(sMat[i] == ele)
            res++;
    }
    error = MPI_Reduce(&res, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0){
        ErrorHandler(error);
        printf("Total number of occurences is: %d\n", result);
    }

    MPI_Finalize();
    exit(0);
}