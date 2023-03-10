#include <mpi.h>
#include <stdio.h>

char* tog(char str[], int rank){
    if(str[rank] >= 97 && str[rank] <= 122){
        str[rank] -= 32;
    }
    else if(str[rank] >= 65 && str[rank] <= 90){
        str[rank] += 32;
    }
    return str;
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char str[] = "HeLLO";
    char printStr[] = "HeLLO";
    printf("Input is : %s, Rank : %d, Output : %s\n", printStr, rank, tog(str, rank));

    MPI_Finalize();
    return 0;
}