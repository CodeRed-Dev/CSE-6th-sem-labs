__kernel void stringManipulation(__global char*A,__global char* C,int len)
{
	int idx=get_global_id(0);
	int size =get_global_size(0);
	int i;
	int k=idx*len;
	for(i=0;i<len;i++){
		C[k++]=A[i];
	}
}