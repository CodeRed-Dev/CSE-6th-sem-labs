import pandas as pd
import numpy as np
dates=pd.date_range('20130101',periods=100)
df=pd.DataFrame(np.random.randn(100,4),index=dates,columns=list('ABCD'))
print(df.head())
print(df.tail())
print(df.index)
print(df.columns)
print(df.T)
print(df.sort_index(axis=1,ascending=False))
print(df.sort_values(by='B'))
print(df[0:3])
#this command of slicing is not working
#print(df['20130105':'201301110'])
print(df.iloc[0,:2])
print(df.iloc[0,0])
print(df['A'])
print(df[['A','B']])

