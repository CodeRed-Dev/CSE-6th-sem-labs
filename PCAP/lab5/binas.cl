
// TODO: Add OpenCL kernel code here.
__kernel void binas(__global char* A,__global char* C)
{
	int idx=get_global_id(0);
	if(A[idx]=='0')
	 C[idx]='1';
	 else if(A[idx]=='1')
	 C[idx]='0';
	 else
	 C[idx]=A[idx];
}
	
	
