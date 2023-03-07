#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])  {
int i,id_rank,nop,j;
int data[100];
int data1;
 int buffer_size = 96;
        int *buffer=malloc(buffer_size*sizeof(int));  
        int funcreturn,ver,subver;
MPI_Status st;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &nop);
MPI_Comm_rank(MPI_COMM_WORLD, &id_rank);
//MPI_Get_version(&ver,&subver);
//printf("the version and subversion of this MPI implementation %d and %d\n",ver,subver);
//printf("%d",MPI_BSEND_OVERHEAD);
if(id_rank==0)
{

printf("Enter the values in the array to be sent to the receivers\n");
for(i=0;i<nop-1;i++)
scanf("%d",&data[i]);
for(i=1,j=0;i<nop;i++,j++)
{
MPI_Buffer_attach(buffer,buffer_size);
funcreturn=MPI_Bsend((data+j),1,MPI_INT,i,1,MPI_COMM_WORLD);
//printf("the Bsend as returned %d as status code\n",funcreturn);
fprintf(stdout,"i %d process have sent the data %d to the process %d \n",id_rank,*(data+j),i);
MPI_Buffer_detach(&buffer,&buffer_size);
fflush(stdout);
}
 //MPI_Buffer_detach(&buffer,&buffer_size);
 int *b=buffer;
}
else
{

MPI_Recv(&data1,1,MPI_INT,0,1,MPI_COMM_WORLD,&st);
//fprintf(stdout, " the data by process %d received is %d\n",id_rank,data1);
if(id_rank%2==0)
{
int res=data1*data1;
printf("i am an even process %d and squared result is %d\n",id_rank,res);
}
else 
{
int res=data1*data1*data1;
printf("i am an odd process %d and cubed result is %d\n",id_rank,res);
}
 }
MPI_Finalize();
return 0;
}