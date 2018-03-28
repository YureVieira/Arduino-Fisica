import matplotlib.pylab as plt
import numpy as np

x1 = np.array([0.212, 0.288, 0.354, 0.405])
y1 = np.array([0.222, 0.385, 0.585, 0.78])

plt.plot(x1,y1,'r-')
plt.plot(x1,y1,'ro')
plt.xlabel(u'Tempo')
plt.ylabel(u'Espaço')

plt.grid(True)
plt.show()
