import random
fptr=open("random.txt","w+")
for i in range(1000):
	line=str(random.randrange(1,100))
	fptr.write(line+"\t")
fptr.close()