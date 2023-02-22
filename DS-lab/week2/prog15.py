import pandas as pd
data = {'Name':['Kavitha', 'Sudha', 'Raju','Vignesh'], 'Height':[150,164,122,178], 'Qualification':['4th fail', 'women','12th pass','Btech']}
df = pd.DataFrame(data, columns=['Name','Height','Qualification'])
lst = ["manipal","amsterdam", "chennai", "mumbai"]
df["Address"]=lst
print(df)