__kernel void convert(__global int *A,__global int *C)
{
    int idx=get_global_id(0);
    int sum=0,p=0,k,power=1;
    while(A[idx])
    {
        k=A[idx]%10;
        for(int i=0;i<=p;i++)
        power*=2;

        sum+=(k*power);
        p++;
        A[idx]/=10;
    }    
    C[idx]=sum;
}