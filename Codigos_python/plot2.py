import matplotlib.pylab as plt
import numpy as np

x1 = np.array([0.212, 0.288, 0.354, 0.405])
y1 = np.array([0.222, 0.385, 0.585, 0.78])

#####cor do fundo do grafico
fig = plt.figure()
rect = fig.patch
rect.set_facecolor('#A4FFB0')
#rect.set_facecolor('white')

ax1 = fig.add_subplot(1, 1, 1, axisbg='white')
ax1.tick_params(axis='x', colors='black',)
ax1.tick_params(axis='y', colors='black')

ax1.set_title(u'Espaço x tempo', color='black', fontsize='20') 
ax1.set_xlabel('tempo(segundos)', color='black', fontsize='20')
ax1.set_ylabel(u'espaço(metros) ', color='black', fontsize='20')

##plt.title('Trilho horizontal')
###plt.title(r'$\sigma_i=23$')
plt.plot(x1,y1,'r-')
plt.plot(x1,y1,'ro')
##plt.xlabel(u'Tempo', fontsize='large')
##plt.ylabel(u'Espaço',fontsize='large')
#plt.text(x1[0]+0.01,y1[0]+0.01,'('+str(x1[0])+', '+str(y1[0])+')',size='small')
#plt.axis((0,0.5,0,1))
#plt.annotate(u'máximo local', xy=(x1[0], y1[0]), xytext=(x1[0], x1[0]), arrowprops=dict(facecolor='black',shrink=0.05)) 

plt.grid(True)
plt.show()
