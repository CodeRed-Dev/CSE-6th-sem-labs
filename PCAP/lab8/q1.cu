#include<cuda.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

__global__ void word_count_kernel(char* str,char* key,int* word_indices,int* result)
{
    int idx=threadIdx.x+blockIdx.x*blockDim.x;
    int p=word_indices[idx];
    int n=word_indices[idx+1];
    char word[100];
    int i=0; 
    for(i=0;i<(n-p-1);i++)
    {
        word[i]=str[p+1+i]; //Extract the word from the input string
    }
    word[i]='\0'; //Terminator character for the word string
    int i1 = 0;
    int i2 = 0;
    int is_equal = 1;

    //Check if the word matches with the key word
    while (word[i1] != '\0' && key[i2] != '\0')
    {

        if (word[i1] == key[i2])
            {
                i1++;
                i2++;
            }else{
                is_equal = 0;
                break;
                }
    }

    if (is_equal == 1)
    {
        atomicAdd(result, 1); //increment result atomically if the word matches with the key word
    }
}

int main()
{
    char str[100]="I scream, you scream for ice cream"; //Input string
    char key[100]="scream"; //Key word
    //enter the string  
    //scanf("%s",str);
    int str_len=strlen(str); //Calculate the length of the input string
    int key_len=strlen(key); //Calculate the length of the key word
    int wc=0;
    for(int i=0;i<str_len;i++)
    {
        if(str[i]==' ')
        {
            wc++; //Count the number of spaces to determine the number of words in the input string
        }
    }
    wc--; //Exclude the last word
    int result=0; //Initialize the result to 0
    int* word_indices=(int*)(malloc(wc*sizeof(int))); //Allocate memory to store the indices of the words in the input string
    int wi=0;
    for(int i=0;i<str_len;i++)
    {
        if(str[i]==' '){
        word_indices[wi++]=i; //Store the index of the word in the input string
        }
    }
    
    char *d_str,*d_key;
    int *d_word_incdies,*d_result;
    cudaMalloc((void**)&d_str,str_len*sizeof(char)); //Allocate memory for input string on the device
    cudaMalloc((void**)&d_key,key_len*sizeof(char)); //Allocate memory for the key word on the device
    cudaMalloc((void**)&d_word_incdies,(wc+1)*sizeof(int)); //Allocate memory for the word indices on the device
    cudaMalloc((void**)&d_result,sizeof(int)); //Allocate memory for the result on the device
    cudaMemcpy(d_str,str,str_len*sizeof(char),cudaMemcpyHostToDevice); //Copy input string to device memory
    cudaMemcpy(d_key,key,key_len*sizeof(char),cudaMemcpyHostToDevice); //Copy key word to device memory
    cudaMemcpy(d_word_incdies,word_indices,(wc+1)*sizeof(int),cudaMemcpyHostToDevice); //Copy word indices to device memory
    cudaMemcpy(d_result,&result,sizeof(int),cudaMemcpyHostToDevice); //Copy result to device memory
    word_count_kernel<<<1,wc>>>(d_str,d_key,d_word_incdies,d_result); //Invoke the kernel with a single block and number of threads equal to
    cudaMemcpy(&result,d_result,sizeof(int),cudaMemcpyDeviceToHost);
    printf("%d ",result);
    cudaFree(d_str);
    cudaFree(d_key);
    cudaFree(d_result);
}
