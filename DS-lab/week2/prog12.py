lst = [-1, 2, -3, 4, -5, 6]
num = 0
print(lst)
while(num < len(lst)):
    if lst[num] < 0:
        print(lst[num], end=" ")
    num += 1