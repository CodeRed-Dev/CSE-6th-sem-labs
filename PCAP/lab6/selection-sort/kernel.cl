__kernel void selectionSort(__global int* A, __global int* C)
{
	int idx=get_global_id(0);
	int size=get_global_size(0);
	int i,pos=0;
	int data=A[idx];
	for(i=0;i<size;i++){
		if((A[i]<data) || (A[i]==data && i<idx)){
			pos++;

		}
	}
	C[pos]=data;

}