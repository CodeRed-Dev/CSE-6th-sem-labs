tup = (12, 7, 38, 56, 78 )
tup1 = ()
print("Tuple = ", tup)
print("The Even Numbers in the tuple are :")
for i in tup:
    if i % 2 == 0:
        tup1 = tup1+(i,)
print(tup1)        