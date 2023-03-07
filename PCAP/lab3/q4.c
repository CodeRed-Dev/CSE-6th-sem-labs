
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#define MCW MPI_COMM_WORLD

int main(int argc, char *argv[])
{

    int rank, size, n = 3, length;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    char A[size * n], B[size * n], *strA, *strB, result[2 * n * size + 1];
    if (rank == 0)
    {
        printf("Enter a string of length %d : ", size * n);
        scanf("%s", A);
        printf("Enter a string of length %d : ", size * n);
        scanf("%s", B);
        length = strlen(A);
        length /= size;
    }
    MPI_Bcast(&length, 1, MPI_INT, 0, MCW);
    strA = (char *)calloc(length, sizeof(char));
    strB = (char *)calloc(length, sizeof(char));
    MPI_Scatter(A, length, MPI_CHAR, strA, length, MPI_CHAR, 0, MCW);
    MPI_Scatter(B, length, MPI_CHAR, strB, length, MPI_CHAR, 0, MCW);
    char sendStr[2 * length + 1];
    sendStr[0] = '\0';
    int i = 0, j = 0, k = 0;
    while (i < length || j < length)
    {
        if (k % 2 == 0)
            sendStr[k++] = strA[i++];
        else
            sendStr[k++] = strB[j++];
    }
    sendStr[k] = '\0';
    MPI_Gather(sendStr, 2 * length, MPI_CHAR, result, 2 * length, MPI_CHAR, 0, MCW);
    if (rank == 0)
    {
        result[2 * n * size] = '\0';
        printf("The result is : ");
        for (int i = 0; i < 2 * n * size; i++)
        {
            printf("%c", result[i]);
        }
        printf("\n");
    }

    free(strA);
    free(strB);
    MPI_Finalize();
    return 0;
}
/*
 #include <mpi.h>
 #include <stdio.h> 
 #include <string.h> 
int main(int argc,char *argv[]) 
{       int rank;    
int N;
int size;      
char str1[100];     
char str2[100];   
char str3[100];  
char B[100];     
char c1[100];      
char c2[100];  
char str4[100];     
int i;       
int M;     
int pointer1;   
pointer1=0;  
int pointer2;     
pointer2=0;  
int count;     
count=0; 
int average;     
MPI_Init(&argc,&argv);     
MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
MPI_Comm_size(MPI_COMM_WORLD,&size);   
if(rank==0)   
{      
	N=size;        
	printf("Give input for string1\n"); 
	scanf("%s",str1);  
	printf("Give input for string2\n");          
	scanf("%s",str2);        
	M=strlen(str1);         
	printf("\n%d",M);     

}
MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);    
MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);      
MPI_Scatter(str1,M/N,MPI_CHAR,c1,M/N,MPI_CHAR,0,MPI_COMM_WORLD);      
MPI_Scatter(str2,M/N,MPI_CHAR,c2,M/N,MPI_CHAR,0,MPI_COMM_WORLD);    
                                     
i=0;       
for(int i=0;i<strlen(c1)+strlen(c2);i++)   
{       
	if(i%2==0)         
	{           
		str3[i]=c1[pointer1];           
		pointer1++;       
	}       
	else      
	{           
		str3[i]=c2[pointer2];
		pointer2++;  
	}   
} 
printf("%s",str3);     
printf("\n");  
MPI_Gather(&str3,2*M,MPI_CHAR,B,2*M,MPI_CHAR,0,MPI_COMM_WORLD);      
if(rank==0)     
{      
	printf("The result of this gathered in the root is:-\n");   
                           
	printf("%s",B);   
}      

       MPI_Finalize();                                                                                                                                                             
} 
*/