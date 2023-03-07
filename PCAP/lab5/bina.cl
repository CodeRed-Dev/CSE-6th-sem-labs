
// TODO: Add OpenCL kernel code here.
__kernel void bina(__global int* A,__global int* C)
{
	int idx=get_global_id(0);
	int digit;
	int binnum=A[idx];
	int power=1,sum=0;
       while(binnum>0)
	{
          digit=binnum%10;
          if(digit)
           digit=0;
        else
          digit=1;
        sum+=digit*power;
        power=power*10;
      
        binnum/=10;
 }
     C[idx]=sum;
}
	
	
