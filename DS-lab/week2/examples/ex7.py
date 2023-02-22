import pandas as pd
import numpy as np
dates=pd.date_range('20130101',periods=100)
df=pd.DataFrame(np.random.randn(100,4),index=dates,columns=list('ABCD'))
print(df.head())

print(df[df.A>0])

