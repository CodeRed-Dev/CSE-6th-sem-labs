import pandas as pd 
df1 = pd.DataFrame({'Name':['Ram', 'Diya', 'Chandan','James','Alice']})
df2 = pd.DataFrame({'Maths':[80.0,90.0,77.5,87.5,86.5], 'physics':[81.0, 94.0, 74.5, 83.0, 82.5], 'chemistry':[91.5,86.5 ,85.5,90,91.0],'biology':[82.5, 83.5, 84.5,85,93] })
df3 = pd.concat([df1,df2], axis =1)
df3["Total"]=df3.iloc[:,1:].sum(axis=1)
print(df3)