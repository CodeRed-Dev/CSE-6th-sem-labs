import numpy as np
r1 = list(map(int,input("Enter the value of row1: ").strip().split()))
r2 = list(map(int,input("Enter the value of row2: ").strip().split()))
r3 = list(map(int,input("Enter the value of row3: ").strip().split()))
arr = np.array([r1,r2,r3])
print(arr)
print("\n")
print("Transpose of the array is")
tran = arr.transpose()
print(tran)
