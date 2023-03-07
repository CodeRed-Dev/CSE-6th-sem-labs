//TODO Add openCl kernel code here.
__kernel void swap(__global int * A)
{
    int idx=get_global_id(0);
	int temp;
	temp = A[idx*2];
	A[idx*2]=A[idx*2+1];
	A[idx*2+1]=temp;
}