x = int(input('Enter x: '))
y = int(input('Enter y: '))
z = int(input('Enter z: '))

if (x>y>z or x>z>y):
	print ("%d is greatest"%(x))
elif (y>x>z or y>z>x):
	print ("%d is greatest"%(y))
elif (z>y>x or z>x>y):
	print ("%d is greatest"%(z))