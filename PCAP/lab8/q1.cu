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
        word[i]=str[p+1+i];
    }
    word[i]='\0';
    int i1 = 0;
    int i2 = 0;
    int is_equal = 1;

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
        atomicAdd(result, 1);
    }
}

int main()
{
    char str[100]="I scream, you scream for ice cream";
    char key[100]="scream";
    //enter the string  
    //scanf("%s",str);
    int str_len=strlen(str);
    int key_len=strlen(key);
    int wc=0;
    for(int i=0;i<str_len;i++)
    {
        if(str[i]==' ')
        {
            wc++;
        }
    }
    wc--;
    int result=0;
    int* word_indices=(int*)(malloc(wc*sizeof(int)));
    int wi=0;
    for(int i=0;i<str_len;i++)
    {
        if(str[i]==' '){
        word_indices[wi++]=i;
        }
    }
    
    char *d_str,*d_key;
    int *d_word_incdies,*d_result;
    cudaMalloc((void**)&d_str,str_len*sizeof(char));
    cudaMalloc((void**)&d_key,key_len*sizeof(char));
    cudaMalloc((void**)&d_word_incdies,(wc+1)*sizeof(int));
    cudaMalloc((void**)&d_result,sizeof(int));
    cudaMemcpy(d_str,str,str_len*sizeof(char),cudaMemcpyHostToDevice);
    cudaMemcpy(d_key,key,key_len*sizeof(char),cudaMemcpyHostToDevice);
    cudaMemcpy(d_word_incdies,word_indices,(wc+1)*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(d_result,&result,sizeof(int),cudaMemcpyHostToDevice);
    word_count_kernel<<<1,wc>>>(d_str,d_key,d_word_incdies,d_result);
    cudaMemcpy(&result,d_result,sizeof(int),cudaMemcpyDeviceToHost);
    printf("%d ",result);
    cudaFree(d_str);
    cudaFree(d_key);
    cudaFree(d_result);
}