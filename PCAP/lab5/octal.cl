
// TODO: Add OpenCL kernel code here.
__kernel void octal(__global int* A,__global int* C)
{
	int idx=get_global_id(0);
	int i=1,octalnum=0;
	int quotient = A[idx];
 	int octalNumber[100];
    //Storing remainders until number is equal to zero
    while (quotient != 0)
    {
        octalNumber[i++] = quotient % 8;
        quotient = quotient / 8;
    }
 
    //Converting stored remainder values in corresponding octal number
    for (int j = i - 1; j > 0; j--)
        octalnum = octalnum*10 + octalNumber[j];
    C[idx]=octalnum;
}
