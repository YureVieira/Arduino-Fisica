#!/usr/bin/python
# -*- coding: cp1252 -*-
############################################
#Grafico de velocidade instantanea/1 Dado
#referencial:ponto de partida-Plano Inclinado
############################################
import numpy as np
import matplotlib.pyplot as plt


#####1:Deslocamento em funcao do Tempo - sem o zero
x1 = np.array([0.212, 0.288, 0.354, 0.405])
y1 = np.array([0.222, 0.385, 0.354, 0.405])

#####cor do fundo do grafico
fig = plt.figure()
rect = fig.patch
#rect.set_facecolor('#A4FFB0')
rect.set_facecolor('white')

ax1 = fig.add_subplot(1, 1, 1, axisbg='white')
ax1.tick_params(axis='x', colors='black',)
ax1.tick_params(axis='y', colors='black')

ax1.set_title('Velocidade x tempo', color='black', fontsize='20') 
ax1.set_xlabel('tempo(milisegundos)', color='black', fontsize='20')
ax1.set_ylabel('velocidade (m/s)', color='black', fontsize='20')


xx=np.arange(0.0, 2500, 50)
z = np.polyfit(x1, y1, 2)
print ('aceleração =')
print 2*z[0]*1000000
plt.plot(xx,1000*(2*z[0]*xx+z[1]))
print ('velocidade (m/s)=')
print 2*z[0]*xx*1000
plt.grid(True)
plt.show()
