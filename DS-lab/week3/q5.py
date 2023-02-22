import numpy as np
print("Enter value for array1: \n")
r1 = list(map(int,input("Enter the value of row1: ").strip().split()))
r2 = list(map(int,input("Enter the value of row2: ").strip().split()))

arr1 = np.array([r1,r2])
print(arr1)
print("Enter value for array2: \n")
rx1 = list(map(int,input("Enter the value of row1: ").strip().split()))
rx2 = list(map(int,input("Enter the value of row2: ").strip().split()))

arr2 = np.array([rx1,rx2])
print(arr2)
print("Sum of arrays is ")
arr3 = np.add(arr1, arr2)
print(arr3)