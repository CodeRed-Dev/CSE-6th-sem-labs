__kernel void even(__global int *a)
{
    int idx = get_global_id(0);
    int size = get_global_size(0);
    int temp;
    if ((idx % 2 == 0) && ((idx + 1) < size))
    {
        if (a[idx] > a[idx + 1])
        {
            temp = a[idx];
            a[idx] = a[idx + 1];
            a[idx + 1] = temp;
        }
    }
}
__kernel void odd(__global int *a)
{
    int idx = get_global_id(0);
    int size = get_global_size(0);
    int temp;
    if ((idx % 2 != 0) && ((idx + 1) < size))
    {
        if (a[idx] > a[idx + 1])
        {
            temp = a[idx];
            a[idx] = a[idx + 1];
            a[idx + 1] = temp;
        }
    }
}