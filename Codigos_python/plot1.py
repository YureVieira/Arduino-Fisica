import matplotlib.pylab as plt
import numpy as np

X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
C,S = np.cos(X), np.sin(X)

fig = plt.figure()
ax = fig.gca()
plt.grid(linestyle=':', linewidth=1)

plt.subplot(2,2,1)
plt.plot(X, C,'r-', label="cosine")
plt.legend(loc='upper left')

plt.subplot(2,2,4)
plt.plot(X, S,'b-', label="sine")
plt.legend(loc='upper left')

plt.axis('tight')

plt.show()
