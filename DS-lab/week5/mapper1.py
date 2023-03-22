"""maper.py"""

import sys
import pandas as pd

df = pd.read_csv('heart_disease_data.csv')

for chol in df['chol']:
    print('%s\t%s' %(chol,1))