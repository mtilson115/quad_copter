import numpy as np
import matplotlib.pyplot as plt  # To visualize
import pandas as pd  # To read data
from sklearn.linear_model import LinearRegression
import sys

cal_file = sys.argv[1]
print(cal_file)

def regress(name,X,Y):
    linear_regressor = LinearRegression()  # create object for the class
    reg = linear_regressor.fit(X,Y)  # perform linear regression
    score = reg.score(X,Y)
    pred = linear_regressor.predict(X)  # make predictions
    print("{} R val = {}".format(name,score))
    print("offsets.{} = (int16_t)({}*((float)t) + {});".format(name,reg.coef_[0][0],reg.intercept_[0]))
    return pred

data = pd.read_csv(cal_file)  # load data set
temp = data.iloc[:, 6].values.reshape(-1, 1)  # values converts it into a numpy array -1 means that calculate the dimension of rows, but have 1 column
Ax = data.iloc[:, 0].values.reshape(-1, 1)
Ay = data.iloc[:, 1].values.reshape(-1, 1)
Az = data.iloc[:, 2].values.reshape(-1, 1)
Gx = data.iloc[:, 3].values.reshape(-1, 1)
Gy = data.iloc[:, 4].values.reshape(-1, 1)
Gz = data.iloc[:, 5].values.reshape(-1, 1)

Ax_pred = regress('ax',temp,Ax)
Ay_pred = regress('ay',temp,Ay)
Az_pred = regress('az',temp,Az)
Gx_pred = regress('gx',temp,Gx)
Gy_pred = regress('gy',temp,Gy)
Gz_pred = regress('gz',temp,Gz)

fig, axs = plt.subplots(3,2)
axs[0,0].scatter(temp,Ax)
axs[0,0].plot(temp,Ax_pred,color='red')
axs[0,0].set_title('Ax')

axs[1,0].scatter(temp,Ay)
axs[1,0].plot(temp,Ay_pred,color='red')
axs[1,0].set_title('Ay')

axs[2,0].scatter(temp,Az)
axs[2,0].plot(temp,Az_pred,color='red')
axs[2,0].set_title('Az')

axs[0,1].scatter(temp,Gx)
axs[0,1].plot(temp,Gx_pred,color='red')
axs[0,1].set_title('Gx')

axs[1,1].scatter(temp,Gy)
axs[1,1].plot(temp,Gy_pred,color='red')
axs[1,1].set_title('Gy')

axs[2,1].scatter(temp,Gz)
axs[2,1].plot(temp,Gz_pred,color='red')
axs[2,1].set_title('Gz')

plt.show()

plt.plot(temp)
plt.show()
