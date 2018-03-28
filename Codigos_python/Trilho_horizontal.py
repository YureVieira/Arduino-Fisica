import matplotlib.pylab as plt
import numpy as np
import serial

#baud rate
b_rate = 115200
#nome da porta criada para o arduino
porta = 'COM22'

ser = serial.Serial(porta,b_rate)
print u'Aguardando o término do experimento...'

temp_1 = ser.readline()
		
temp_2 = ser.readline()

temp_3 = ser.readline()

temp_4 = ser.readline()
#Encerra a conexão
ser.close()

print temp_1,temp_2,temp_3,temp_4

x1 = np.array([float(temp_1),float(temp_2), float(temp_3), float(temp_4)])
y1 = np.array([222, 385, 585, 780])

#cor do fundo do grafico
fig = plt.figure()
rect = fig.patch
rect.set_facecolor('white')

ax1 = fig.add_subplot(1, 1, 1, axisbg='white')
ax1.tick_params(axis='x', colors='black',)
ax1.tick_params(axis='y', colors='black')

ax1.set_title(u'Espaço x tempo', color='black', fontsize='20') 
ax1.set_xlabel('tempo(milisegundos)', color='black', fontsize='20')
ax1.set_ylabel(u'espaço(milimetros) ', color='black', fontsize='20')

plt.plot(x1,y1,linestyle='-', color='green',marker='o', linewidth=3.0)

plt.grid(True)
plt.show()
