/*#include <mpi/mpi.h>
#include <stdio.h>
#include <string.h>

void toggle(int n, char* word){
	for(int i = 0; i<n; i++){
		if(word[i]>='a' && word[i]<='z'){
			word[i] -= 32;
		}
		else if(word[i]>='A' && word[i]<='Z'){
			word[i] += 32;
		}
	}
}*/
#include<mpi.h>
#include<stdio.h>
#include<string.h>

void toggle(int n, char* word);
int main(int argc, char* argv[]){
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char word[100];
	int length;
	MPI_Status status;
	if(rank == 0){
		printf("Enter the word to be sent : ");
		scanf("%s", word);
		length = strlen(word);
		printf("original string : %s\n", word);
		MPI_Ssend(&length, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Ssend(word, length, MPI_CHAR, 1, 1, MPI_COMM_WORLD);  
		MPI_Recv(word, length, MPI_CHAR, 1, 2, MPI_COMM_WORLD, &status);
		printf("toggled string : %s\n", word);
	}
	else{
		MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(word, length, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
		toggle(length, word);
		MPI_Ssend(word, length, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
void toggle(int n, char* word){
	for(int i = 0; i<n; i++){
		if(word[i]>='a' && word[i]<='z'){
			word[i] -= 32;
		}
		else if(word[i]>='A' && word[i]<='Z'){
			word[i] += 32;
		}
	}
}