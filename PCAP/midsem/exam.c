#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc , char *argv[])
{
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int A[4][4],B[4][4];
	int C[4],D[4],small[4],large[4];

	if (rank==0){
		printf("Enter the elements of A \n");
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
			{
				scanf("%d",&A[i][j]);
			}
		}
		printf("Enter the elements of B \n");
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
			{
				scanf("%d",&B[i][j]);
			}
		}
	}

	MPI_Scatter(B,4,MPI_INT,D,4,MPI_INT,0,MPI_COMM_WORLD);
	for(int i=0;i<4;i++){
		MPI_Scatter(&A[i],1,MPI_INT,&C[i],1,MPI_INT,0,MPI_COMM_WORLD);
	}
	int smalli=999999;
	int pos=0;

	for(int i=0;i<4;i++){
		if(D[i]<smalli){
			smalli=D[i];
			pos=i;
		}
	}

	MPI_Gather(&pos,1,MPI_INT,small,1,MPI_INT,0,MPI_COMM_WORLD);

	int largei=0;

	for(int i=0;i<4;i++){
		if(C[i]>largei){
			largei=C[i];
		}
	}

	MPI_Gather(&largei,1,MPI_INT,large,1,MPI_INT,0,MPI_COMM_WORLD);

	if (rank==0){
		for(int i=0;i<4;i++){
			if(B[i][i]!=B[i][small[i]]){
				B[i][i]=0;
			}
		}

		for(int i=0;i<4;i++){
			int val=large[i];
			B[i][small[i]]+=val;
		}

		printf("\nThe result is \n");

		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
			{
				printf("%d ",B[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}