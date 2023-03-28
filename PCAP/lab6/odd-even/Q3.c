#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string.h>
#define MAX_SOURCE_SIZE 1000
int main() // executed by host(CPU)
{
    char *source_str, *programSource;
    size_t source_size;
    FILE *fp = fopen("odd-even-sort.cl", "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to load kernel\n");
        getchar();
        exit(1);
    }
    int n, len[10], i;
    programSource = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(programSource, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    int *A = NULL, *C = NULL; // input and output arrays in host
    fprintf(stdout, "Enter no. of elements and elements: ");
    scanf("%d", &n);
    A = (int *)malloc(n * sizeof(int));
    // C = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        scanf("%d", &A[i]);
    size_t datasize = n * sizeof(int);
    cl_int status;

    // Step 1: Discover and initialize platforms
    cl_uint numPlatforms = 0;
    cl_platform_id *platforms = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    printf("%d platform success %d\n", status, numPlatforms);
    platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);
    char pform_vendor[40];
    clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, sizeof(pform_vendor), pform_vendor, NULL);
    printf("Vendor is %s\n", pform_vendor);

    // Step 2: Discover and initialize devices
    cl_uint numDevices = 0;

    cl_device_id *devices = NULL;
    status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
    devices = (cl_device_id *)malloc(numDevices * sizeof(cl_device_id));
    status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
    printf("%d device success %d\n", status, numDevices);
    char name_data[100];
    int err = clGetDeviceInfo(devices[0], CL_DEVICE_NAME, sizeof(name_data), name_data, NULL);
    printf("Device name is %s", name_data);

    // Step 3: Create context
    cl_context context = NULL;
    context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);
    printf("%d context success %d\n", status, numDevices);

    // Step 4: Create command queue (1 per device)
    cl_command_queue cmdQueue;
    cmdQueue = clCreateCommandQueue(context, devices[0], 0, &status);
    printf("%d command queue success %d\n", status, numDevices);

    // Step 5: Create device buffers
    cl_mem bufferA, bufferC; // input and output array in device
    bufferA = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize, NULL, &status);
    // bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize, NULL, &status);

    // Step 6: Write host data to device buffers
    status = clEnqueueWriteBuffer(cmdQueue, bufferA, CL_TRUE, 0, datasize, A, 0, NULL, NULL);
    printf("%d write buffer done\n", status);

    // Step 7: Create and compile program
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&programSource, (const size_t *)&source_size, &status);
    status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);
    printf("%d build done\n", status);

    // Step 8: Create kernel
    cl_kernel odd_kernel, even_kernel = NULL;
    odd_kernel = clCreateKernel(program, "odd", &status);
    even_kernel = clCreateKernel(program, "even", &status);
    printf("%d kernel created\n", status);

    // Step 9: Pass kernel arguments
    status = clSetKernelArg(odd_kernel, 0, sizeof(cl_mem), &bufferA);
    status = clSetKernelArg(even_kernel, 0, sizeof(cl_mem), &bufferA);
    printf("%d arguments passed\n", status);

    // Step 10: Config. work-item struct
    size_t globalWorkSize[1];
    globalWorkSize[0] = n;

    // Step 11: Enqueue kernel for execution
    for (i = 0; i < n / 2; i++)
    {
        status = clEnqueueNDRangeKernel(cmdQueue, odd_kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
        status = clEnqueueNDRangeKernel(cmdQueue, even_kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    }

    // Step 12: Read output buffer back to host
    clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0, datasize, A, 0, NULL, NULL);
    printf("Output array:\n");
    for (i = 0; i < n; i++)
        printf("%d\n", A[i]);

    // Step 13: Release resources
    // Free OpenCL resources
    clReleaseKernel(odd_kernel);
    clReleaseKernel(even_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufferA);
    // clReleaseMemObject(bufferC);
    clReleaseContext(context);
    // Free host resources
    free(A);
    // free(C);
    free(platforms);
    free(devices);
}