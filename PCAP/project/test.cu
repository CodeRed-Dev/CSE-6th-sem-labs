#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

#define BLOCK_SIZE 32

__global__ void reverseGrayLevel(unsigned char *img, int numRows, int numCols)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < numRows && col < numCols)
    {
        int index = row * numCols + col;
        img[index] = 255 - img[index];
    }
}

int main(int argc, char **argv)
{
    // Check for correct number of arguments
    if (argc != 3)
    {
        printf("Usage: %s input_image output_image\n", argv[0]);
        exit(1);
    }
    
    // Open input image file
    FILE *fp = fopen(argv[1], "rb");
    if (!fp)
    {
        printf("Could not open input image file %s\n", argv[1]);
        exit(1);
    }
    
    // Read in PGM header information
    char magic[3];
    int numCols, numRows, maxVal;
    fscanf(fp, "%2s %d %d %d", magic, &numCols, &numRows, &maxVal);
    
    // Allocate memory for input and output images
    int imageSize = numRows * numCols;
    unsigned char *h_inputImg = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    unsigned char *h_outputImg = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    unsigned char *d_inputImg, *d_outputImg;
    cudaMalloc(&d_inputImg, imageSize * sizeof(unsigned char));
    cudaMalloc(&d_outputImg, imageSize * sizeof(unsigned char));
    
    // Read in input image data
    fread(h_inputImg, sizeof(unsigned char), imageSize, fp);
    fclose(fp);
    
    // Copy input image data to device
    cudaMemcpy(d_inputImg, h_inputImg, imageSize * sizeof(unsigned char), cudaMemcpyHostToDevice);
    
    // Set up grid and block sizes
    dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE, 1);
    dim3 gridSize((numCols + BLOCK_SIZE - 1) / BLOCK_SIZE, (numRows + BLOCK_SIZE - 1) / BLOCK_SIZE, 1);
    
    // Call kernel to reverse gray-level of image
    reverseGrayLevel<<<gridSize, blockSize>>>(d_inputImg, numRows, numCols);
    
    // Copy result back to host
    cudaMemcpy(h_outputImg, d_inputImg, imageSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);
    
    // Write out result to output image file
    fp = fopen(argv[2], "wb");
    fprintf(fp, "%s\n%d %d\n%d\n", magic, numCols, numRows, maxVal);
    fwrite(h_outputImg, sizeof(unsigned char), imageSize, fp);
    fclose(fp);
    
    // Free memory
    free(h_inputImg);
    free(h_outputImg);
    cudaFree(d_inputImg);
    cudaFree(d_outputImg);
    
    return 0;
}
//nvcc -o reverse_gray_level_image reverse_gray_level_image.cu
// ./reverse_gray_level_image input_image.pgm output_image.pgm
