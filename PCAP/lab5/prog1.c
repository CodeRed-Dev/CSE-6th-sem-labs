#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CL/cl.h>
#define MAX_SCORE_SIZE 1000

int main(){
	char *source_str;
	size_t source_size;
	char *programSource;
	FILE* fp;
	fp=fopen("swap1.cl","r");
	if(!fp){
		fprintf(stderr,"Failed to load kernel\n");
		exit(1);
	}
	int n,len[10],i;
	programSource=(char *)malloc(MAX_SCORE_SIZE);
	source_size=fread(programSource,1,MAX_SCORE_SIZE,fp);
	fclose(fp);
	int *A=NULL;
	int *C=NULL;
	printf("Enter the number of elements in the array : ");
	scanf("%d",&n);
	printf("Enter the elements \n");
	A=(int*)malloc(sizeof(int)*n);
	C=(int*)malloc(sizeof(int)*n);
	for(i=0;i<n;i++){
		scanf("%d",&A[i]);
	}
	size_t datasize = sizeof(int)*n;
	printf("size of A is %lu \n",datasize);
	printf("The elements are \n");
	for(i=0;i<n;i++){
		printf("%d ",A[i]);
	}

	//STEP1 

	cl_int status;
	cl_uint numPlatforms =0;
	cl_platform_id *platforms=NULL;
	status=clGetPlatformIDs(0,NULL,&numPlatforms);
	printf("\n%d platform success %d \n",status,numPlatforms);
	platforms=(cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
	status=clGetPlatformIDs(numPlatforms,platforms,NULL);
	char pform_vendor[40];
	clGetPlatformInfo(platforms[0],CL_PLATFORM_NAME,sizeof(pform_vendor),&pform_vendor,NULL);
	printf("The vendor %s \n",pform_vendor);

	//STEP2

	cl_uint numDevices=0;
	cl_device_id *devices=NULL;
	status = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,0,NULL,&numDevices);
	devices=(cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
	status = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,numDevices,devices,NULL);
	printf("%d Device success %d \n",status,numDevices);
	char name_data[100];
	int err = clGetDeviceInfo(devices[0],CL_DEVICE_NAME,sizeof(name_data),name_data,NULL);
	printf("The device name %s \n",name_data);

	//STEP3- CREATE CONTEXT

	cl_context context = NULL;
	context=clCreateContext(NULL,numDevices,devices,NULL,NULL,&status);
	printf("%d context success %d \n",status,numDevices);

	//STEP 4 - COMMAND QUEUE
	cl_command_queue cmdQueue;
	cmdQueue= clCreateCommandQueue(context,devices[0],0,&status);
	printf("%d CQ success %d\n",status,numDevices);

	//STEP-5 DEVICE BUFFERS

	cl_mem bufferA;
	cl_mem bufferC;
	bufferA=clCreateBuffer(context,CL_MEM_READ_WRITE,datasize,NULL,&status);
	//bufferC=clCreateBuffer(context,CL_MEM_WRITE,datasize,NULL,&status);

	//STEP 6 - WRITE HOST DATA TO DEVICE BUFFERS

	status=clEnqueueWriteBuffer(cmdQueue,bufferA,CL_FALSE,0,datasize,A,0,NULL,NULL);
	printf("wb done %d \n",status);

	//STEP7- CREATE AND COMPILE PROGRAM

	cl_program program = clCreateProgramWithSource(context,1,(const char**)&programSource,(const size_t*)&source_size,&status);
	status=clBuildProgram(program,numDevices,devices,NULL,NULL,NULL);
	printf("bl done %d \n",status);

	//Step8 -CREATE KERNEL

	cl_kernel kernel=  NULL;
	kernel=clCreateKernel(program,"swap",&status);
	printf("kr done %s \n",status);

	//STEP9- KERNEL ARGUMENTS

	status=clSetKernelArg(kernel,0,sizeof(cl_mem),&bufferA);
	// status=clSetKernelArg(kernel,0,sizeof(cl_mem),&bufferC);

	printf("ka done %s\n",status );

	//STEP10 - WORK-TIME STRUCTURE 

	size_t globalWorkSize[1];
	globalWorkSize[0]=n/2;

	//STEP 11 - ENQUEUE THE KERNEL FOR EXECUTION

	status=clEnqueueNDRangeKernel(cmdQueue,kernel,1,NULL,globalWorkSize,NULL,0,NULL,NULL);

	//STEP12 - READ THE OUTPUT BUFFER BACK TO THE HOST

	clEnqueueReadBuffer(cmdQueue,bufferA,CL_TRUE,0,datasize,A,0,NULL,NULL);
	printf("the o/p are \n");
	for(i=0;i<n;i++){
		printf("%d ",A[i]);
	}
	puts("");
	//STEP13 - RELEASE OpenCL RESOURCES

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(bufferA);
	//	clReleaseMemObject(bufferC);
	clReleaseContext(context);
	free(A);
	free(platforms);
	free(devices);
}