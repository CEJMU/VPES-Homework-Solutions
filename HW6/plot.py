import seaborn as sns
import polars as pl
import matplotlib.pyplot as plt

df = pl.read_csv("results.csv");
print(df)

sns.barplot(data=df,x="quantum",y="time")
plt.show()
