#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kernel function to copy substrings of input string to output string
__global__ void kernel(char* sin, int* sin_len, char* sout)
{
    // Calculate the global index for the current thread
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    int si = 0; // start index
    for (int i = 0; i < idx; i++)
    {
        // Calculate the start index for the substring that this thread will copy
        si += (*sin_len)-i;
    }

    // Calculate the total number of characters in the substring that this thread will copy
    int total_chars = (*sin_len) - idx;
    for (int i = 0; i < total_chars; i++)
    {
        // Copy each character of the substring from input to output string
        sout[si++] = sin[i];
    }
}

int main()
{
    char sin[100] = "PCAP"; // Input string
    char sout[100]; // Output string

    int sin_len = strlen(sin); // Length of input string
    int sout_len = 0;

    for (int i = 0; i < sin_len; i++)
    {
        sout_len += (i+1); // Calculate the total length of output string
    }

    // Allocate memory on device for input string, input string length, and output string
    char* d_sin;
    int* d_sin_len;
    char* d_sout;

    cudaMalloc((void**) &d_sin, sin_len * sizeof(char));
    cudaMalloc((void**) &d_sin_len, sizeof(int));
    cudaMalloc((void**) &d_sout, (sout_len + 1) * sizeof(char)); // Add 1 to allocate space for null terminator

    // Copy input string, input string length, and output string to device
    cudaMemcpy(d_sin, sin, sin_len * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sin_len, &sin_len, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sout, sout, (sout_len + 1) * sizeof(char), cudaMemcpyHostToDevice);

    // Call kernel function with 1 block and sin_len threads
    kernel<<<1, sin_len>>>(d_sin, d_sin_len, d_sout);

    // Copy output string from device to host
    cudaMemcpy(sout, d_sout, (sout_len + 1) * sizeof(char), cudaMemcpyDeviceToHost);

    // Add null terminator to output string
    sout[sout_len] = '\0';

    // Print input and output strings
    printf("String-input: %s\n", sin);
    printf("String-output: %s\n", sout);

    // Free device memory
    cudaFree(d_sin);
    cudaFree(d_sin_len);
    cudaFree(d_sout);

    return 0;
}
