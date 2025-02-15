import numpy as np

x = np.linspace(0.0,3.0,10000)
y = (1/(1+2*x**2+x**4)+1.0)**0.5
dx = x[1] - x[0]                        #每个矩形的宽度
fArea = np.sum(y*dx)                    #矩形宽*高，再求和
print("Integral area:",fArea)
