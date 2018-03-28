import matplotlib.pylab as plt
import numpy as np
import serial

b_rate = 115200 #baud rate
porta = 'COM22'  #nome da porta criada para o arduino
ser = serial.Serial(porta,b_rate)
print 'Aguardando o termino do experimento...'

temp_1 = ser.readline()
		
temp_2 = ser.readline()

temp_3 = ser.readline()

temp_4 = ser.readline()
ser.close()
print temp_1,temp_2,temp_3,temp_4

x1 = np.array([float(temp_1),float(temp_2), float(temp_3), float(temp_4)])
y1 = np.array([222, 385, 585, 780])

#####cor do fundo do grafico
fig = plt.figure()
rect = fig.patch
rect.set_facecolor('white')

ax1 = fig.add_subplot(1, 1, 1, axisbg='white')
ax1.tick_params(axis='x', colors='black',)
ax1.tick_params(axis='y', colors='black')

ax1.set_title(u'Espa�o x tempo', color='black', fontsize='20') 
ax1.set_xlabel('tempo(milisegundos)', color='black', fontsize='20')
ax1.set_ylabel(u'espa�o(milimetros) ', color='black', fontsize='20')

plt.plot(x1,y1,linestyle='-', color='green',marker='o', linewidth=3.0)
##plt.plot(x1,y1,'ro')

##plt.bar(x1,y1, width=10, color='red',align='center')

plt.grid(True)
plt.show()
