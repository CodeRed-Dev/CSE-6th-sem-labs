import pandas as pd
data = {'Name':['Annie', 'Diya', 'Charles','James','Emily'], 'Quiz_1/10':[8,9,7.5,8.5,6.5], 'In-sem1/15':[11,14,14.5,13,12.5],'Quiz_2/10':[9.5,6.5,8.5,9,9],'In-sem2/15':[12.5,13.5,14.5,15,13]}
df = pd.DataFrame(data, columns=['Name','Quiz_1/10','In-sem1/15','Quiz_2/10','In-sem2/15'], index=range(1,6))
df["Total"]=df.iloc[:,1:].sum(axis=1)
df2 = df.iloc[:,1:].sum(axis=0)/len(df.index)
df.loc["Mean"] = df2
print(df)