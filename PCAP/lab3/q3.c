#include<mpi.h>
#include<stdio.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int rank,size;
	char str[100];
	int B[10];
	char C[20];
	int len;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank==0){
		printf("Enter the string \n");
		scanf("%s",str);
		puts("");
		len=strlen(str);
	}

	MPI_Bcast(&len,1,MPI_INT,0,MPI_COMM_WORLD);
	len/=size;
	MPI_Scatter(str,len,MPI_CHAR,C,len,MPI_CHAR,0,MPI_COMM_WORLD);
	printf("Process %d received substring : ",rank);
	int cnt=0;
	for(int i=0;i<len;i++){
		printf("%c ",C[i]);
		cnt+=1;
		if( C[i]=='A' || C[i]=='a' || C[i]=='E' || C[i]=='e' || C[i]=='I' || C[i]=='i' || C[i]=='O' || C[i]=='o' || C[i]=='U' || C[i]=='u'){
			cnt-=1;
		}
	}

	puts("");
	MPI_Gather(&cnt,1,MPI_INT,&B,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		int tcnt=0;
		printf("The values gathered in root process are - \n");
		for(int i=0;i<size;i++){
			printf("%d ",B[i]);
			tcnt+=B[i];
		}
		puts("");
		printf("Total count is - %d ",tcnt);
		puts("");
	}
	return 0;
}