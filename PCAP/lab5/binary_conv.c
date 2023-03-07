#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string.h>
#define MAX_SOURCE_SIZE 1000

int main()
{
    char *source_str;
    size_t source_size;
    char* programSource;
    FILE *fp;
    fp=fopen("binary_conv.cl","r");
    if(!fp)
    {
        fprintf(stderr,"Failed to load kernel.\n");
        getchar();
        exit(1);
    }

    int n,len[10],i;
    programSource=(char*)malloc(MAX_SOURCE_SIZE);
    source_size=fread(programSource,1,MAX_SOURCE_SIZE,fp);
    fclose(fp);
    
    int *A=NULL;
    int *C=NULL;
    fprintf(stdout,"Enter the number of elements : ");
    scanf("%d",&n);
    A=(int *)malloc(sizeof(int)*n);
    C=(int *)malloc(sizeof(int)*n);
    printf("Populate the array : ");
    for(i=0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }

    printf("The elements are\n");
    for(i=0;i<n;i++)
    {
        printf("%d ",A[i]);
    }
    size_t datasize=sizeof(int)*n;
    printf("\nsize of A is %lu\n",datasize);
    
    cl_int status;

//step1
    cl_uint numPlatforms=0;
    cl_platform_id *platforms=NULL;
    status=clGetPlatformIDs(0,NULL,&numPlatforms);
    printf("%d platform success %d\n",status,numPlatforms);
    platforms=(cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
    status=clGetPlatformIDs(numPlatforms,platforms,NULL);
    char pform_vendor[40];
    clGetPlatformInfo(platforms[0],CL_PLATFORM_NAME,sizeof(pform_vendor),&pform_vendor,NULL);
    printf("The vendor %s\n",pform_vendor);

//step2
    cl_uint numDevices=0;
    cl_device_id *devices=NULL;
    status=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,0,NULL,&numDevices);
    devices=(cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
    status=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,numDevices,devices,NULL);
    printf("%d Device success %d\n",status,numDevices);
    char name_data[100];
    int err=clGetDeviceInfo(devices[0],CL_DEVICE_NAME,sizeof(name_data),name_data,NULL);
    printf("The device name %s\n",name_data);

//step3
    cl_context context=NULL;//context : how many devices we are targetting and the device type
    context=clCreateContext(NULL,numDevices,devices,NULL,NULL,&status);
    printf("%d context success %d\n",status, numDevices);

//step4
    cl_command_queue cmdQueue;//for n devices we have to create n cmd queues
    cmdQueue=clCreateCommandQueue(context,devices[0],0,&status);
    printf("%d CQ success %d\n",status,numDevices);

//step5
    cl_mem bufferA;
    cl_mem bufferC;
    bufferA=clCreateBuffer(context,CL_MEM_READ_ONLY,datasize,NULL,&status);
    bufferC=clCreateBuffer(context,CL_MEM_WRITE_ONLY,datasize,NULL,&status);

//step6
    status = clEnqueueWriteBuffer(cmdQueue,bufferA,CL_FALSE,0,datasize,A,0,NULL,NULL);
    printf("Write buffer %d\n",status);

//step7
    cl_program program=clCreateProgramWithSource(context,1,(const char**)&programSource,(const size_t*)&source_size,&status);
    status=clBuildProgram(program,numDevices,devices,NULL,NULL,NULL);
    printf("Build status %d\n",status);

//step8
    cl_kernel kernel=NULL;
    kernel=clCreateKernel(program,"convert",&status);
    printf("kernel create status %d\n",status);

//step9
    status = clSetKernelArg(kernel,0,sizeof(cl_mem),&bufferA);
    printf("Kernel arguments status %d\n",status);
    status = clSetKernelArg(kernel,1,sizeof(cl_mem),&bufferC);
    printf("Kernel arguments status %d\n",status);

//step10
    size_t globalWorkSize[1];
    globalWorkSize[0]=n;

//step11
    status = clEnqueueNDRangeKernel(cmdQueue,kernel,1,NULL,globalWorkSize,NULL,0,NULL,NULL);
    printf("Kernel executed %d\n",status);
//step12
    status = clEnqueueReadBuffer(cmdQueue,bufferA,CL_TRUE,0,datasize,A,0,NULL,NULL);
    printf("On conversion : \n");
    for(int i=0;i<n;i++)
    {
        printf("%d -> %d\n",A[i],C[i]);
    }
    printf("\n");

//step13 - Releasing the resource
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferC);
    clReleaseContext(context);
    free(A);
    free(C);
    free(platforms);
    free(devices);
}