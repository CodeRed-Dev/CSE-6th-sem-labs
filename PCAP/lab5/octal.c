#include <stdio.h>
#include <stdlib.h>
// OpenCL includes
#include <CL/cl.h>
//#include<iostream>
#include<string.h>
#define MAX_SOURCE_SIZE 1000
//#include<conio.h>





int main() {
// This code executes on the OpenCL host
// Host data
	char *source_str;
    size_t source_size;
 char* programSource;
	FILE *fp;
	fp=fopen("octal.cl", "r");
    if(!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
		getchar();
        exit(1);
    }
    int n,len[10],i,k,j;
    programSource = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(programSource, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
int *A=NULL; // Input array
  int *C=NULL; // Output array

fprintf(stdout,"ENTER THE NO. OF ELEMENTS : ");
scanf("%d",&n);
A=(int*)malloc(sizeof(int)*n);
  C=(int*)malloc(sizeof(int)*n);
for(i=0;i<n;i++)
{
scanf("%d",&A[i]);
}

// Compute the size of the data
  size_t datasize = sizeof(int)*n;
printf("the size is %d\n",datasize);
printf("the i/p numbers are\n");
for(i=0;i<n;i++){
    printf("%d ",A[i]);
  }

// Use this to check the output of each API call
cl_int status;

// STEP 1: Discover and initialize the platforms

cl_uint numPlatforms = 0;
cl_platform_id *platforms = NULL;
// Use clGetPlatformIDs() to retrieve the number of
// platforms
status = clGetPlatformIDs(0, NULL, &numPlatforms);
printf("\n%d platform success %d \n",status,numPlatforms); 
// Allocate enough space for each platform
platforms =
(cl_platform_id*)malloc(
numPlatforms*sizeof(cl_platform_id));
// Fill in platforms with clGetPlatformIDs()
status = clGetPlatformIDs(numPlatforms, platforms,
NULL);
char pform_vendor[40];
clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, sizeof(pform_vendor),
&pform_vendor, NULL);
printf(" the vendor %s\n",pform_vendor);

// STEP 2: Discover and initialize the devices

cl_uint numDevices = 0;
cl_device_id *devices = NULL;
// Use clGetDeviceIDs() to retrieve the number of
// devices present
status = clGetDeviceIDs(
platforms[0],
CL_DEVICE_TYPE_GPU,
0,
NULL,
&numDevices);
// Allocate enough space for each device
devices =
(cl_device_id*)malloc(
numDevices*sizeof(cl_device_id));
// Fill in devices with clGetDeviceIDs()
status = clGetDeviceIDs(
platforms[0],
CL_DEVICE_TYPE_GPU,
numDevices,
devices,
NULL);
printf("%d Device success %d \n",status,numDevices); 
char name_data[100];
int err = clGetDeviceInfo(devices[0], CL_DEVICE_NAME,
sizeof(name_data), name_data, NULL);
printf(" the device name %s\n",name_data);

// STEP 3: Create a context

cl_context context = NULL;
// Create a context using clCreateContext() and
// associate it with the devices
context = clCreateContext(
NULL,
numDevices,
devices,
NULL,
NULL,
&status);
printf("%d context success %d \n",status,numDevices); 

// STEP 4: Create a command queue

cl_command_queue cmdQueue;
// Create a command queue using clCreateCommandQueue(),
// and associate it with the device you want to execute
// on
cmdQueue = clCreateCommandQueue(
context,
devices[0],
0,
&status);
printf("%d CQ success %d \n",status,numDevices); 

// STEP 5: Create device buffers

cl_mem bufferA; // Input array on the device

cl_mem bufferC; // Output array on the device
// Use clCreateBuffer() to create a buffer object (d_A)
// that will contain the data from the host array A
bufferA = clCreateBuffer(
context,
CL_MEM_READ_ONLY,
datasize,
NULL,
&status);
printf("ba cr %d \n",status);
// Use clCreateBuffer() to create a buffer object (d_C)
// with enough space to hold the output data
bufferC = clCreateBuffer(
context,
CL_MEM_WRITE_ONLY,
datasize,
NULL,
&status);
printf("bc cr %d \n",status);
// STEP 6: Write host data to device buffers

// Use clEnqueueWriteBuffer() to write input array A to
// the device buffer bufferA
status = clEnqueueWriteBuffer(
cmdQueue,
bufferA,
CL_FALSE,
0,
datasize,
A,
0,
NULL,
NULL);

printf("wb  %d \n",status);
// STEP 7: Create and compile the program

// Create a program using clCreateProgramWithSource()
cl_program program = clCreateProgramWithSource(
context,
1,
(const char**)&programSource,
(const size_t *)&source_size,
&status);

// Build (compile) the program for the devices with
// clBuildProgram()
status = clBuildProgram(
program,
numDevices,
devices,
NULL,
NULL,
NULL);
printf("biuld %d \n",status);
// STEP 8: Create the kernel

cl_kernel kernel = NULL;
// Use clCreateKernel() to create a kernel from the
// vector addition function (named "vecadd")
kernel = clCreateKernel(program, "octal", &status);


// STEP 9: Set the kernel arguments

// Associate the input and output buffers with the
// kernel
// using clSetKernelArg()
status = clSetKernelArg(
kernel,
0,
sizeof(cl_mem),
&bufferA);

status |= clSetKernelArg(
kernel,

1,
sizeof(cl_mem),
&bufferC);


// STEP 10: Configure the work-item structure

// Define an index space (global work size) of work
// items for
// execution. A workgroup size (local work size) is not
// required,
// but can be used.
size_t globalWorkSize[1];
// There are ??elements?? work-items
globalWorkSize[0] = datasize;

// STEP 11: Enqueue the kernel for execution

// Execute the kernel by using
// clEnqueueNDRangeKernel().
// ??globalWorkSize?? is the 1D dimension of the
// work-items
status = clEnqueueNDRangeKernel(
cmdQueue,
kernel,
1,
NULL,
globalWorkSize,
NULL,
0,
NULL,
NULL);
printf("ker exe %d \n",status);
// STEP 12: Read the output buffer back to the host

// Use clEnqueueReadBuffer() to read the OpenCL output
// buffer (bufferC)
// to the host output array (C)
clEnqueueReadBuffer(
cmdQueue,
bufferC,
CL_TRUE,
0,
datasize,
C,
0,
NULL,
NULL);
printf("rb %d \n",status);
printf("the o/p ELEMENTS  are\n");
for(i=0;i<n;i++)
{
  printf("%d ",C[i]);
}
puts("");
// STEP 13: Release OpenCL resources

// Free OpenCL resources
clReleaseKernel(kernel);
clReleaseProgram(program);
clReleaseCommandQueue(cmdQueue);
clReleaseMemObject(bufferA);
clReleaseMemObject(bufferC);
clReleaseContext(context);
// Free host resources
//free(A);
//free(C);
free(platforms);
free(devices);

}
