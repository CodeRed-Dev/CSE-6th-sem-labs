list = [11, -21, 0, 45, 66, -93]
print("List is ")
print(list)
pos = 0
neg = 0
for i in list: 
	if i<0:
		neg +=1
	else:
		pos +=1
print("\nNo. of +ve intergers are : %d \nNo. of -ve integers are: %d "%(pos,neg))		

