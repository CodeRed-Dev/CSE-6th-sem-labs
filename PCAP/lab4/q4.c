#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

void ErrorHandler(int error){
    if(error != MPI_SUCCESS){
        char error_string[BUFSIZ];
        int length_err_string, error_class;
        MPI_Error_class(error, &error_class);
        MPI_Error_string(error, error_string, &length_err_string);
        fprintf(stderr, "Error: %d %s\n", error_class, error_string);
        //exit(1);
    }
}

int main(int argc, char* argv[]){
    int rank, size, error;
    int mat[4][4], fmat[4][4];
    MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
    error = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    error = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0)
        ErrorHandler(error);

    if(rank == 0){
        printf("Enter the elements in 4x4 matrix:\n");
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                scanf("%d",&mat[i][j]);
            }
        }
    }

    int mmat[4],smat[4];
    error = MPI_Scatter(mat, 4, MPI_INT, mmat, 4, MPI_INT, 0, MPI_COMM_WORLD);
    error = MPI_Scan(mmat, smat, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    error = MPI_Gather(smat, 4, MPI_INT, fmat, 4, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0){
        ErrorHandler(error);
        printf("The final result is : \n");
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                printf("%d ", fmat[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    exit(0);
}