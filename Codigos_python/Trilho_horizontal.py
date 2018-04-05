import matplotlib.pylab as plt
import numpy as np
import serial

#baud rate
b_rate = 115200
#nome da porta criada para o arduino
porta = 'COM7'

ser = serial.Serial(porta,b_rate)

#Mostra mensagens pre-experimento
while 1:
    if ser.inWaiting():
        mensagem = ser.readline()
        print mensagem
        #Mensagem "[START]\r\n" sinaliza que daqui para frente serão enviados dados dos sensores
        if mensagem == '[START]\r\n':
            print 'Saindo!'
            break
print u'Aguardando o término do experimento...'
#Colhe os 4 tempos
tempo_1 = ser.readline()
		
tempo_2 = ser.readline()

tempo_3 = ser.readline()

tempo_4 = ser.readline()
#Encerra a conexão
ser.close()

print tempo_1,tempo_2,tempo_3,tempo_4

x1 = np.array([float(tempo_1),float(tempo_2), float(tempo_3), float(tempo_4)])
y1 = np.array([222, 385, 585, 780])

#cor do fundo do grafico
fig = plt.figure()
rect = fig.patch
rect.set_facecolor('white')

ax1 = fig.add_subplot(1, 1, 1)
ax1.tick_params(axis='x', colors='black',)
ax1.tick_params(axis='y', colors='black')

ax1.set_title(u'Espaço x tempo', color='black', fontsize='20') 
ax1.set_xlabel('tempo(milisegundos)', color='black', fontsize='20')
ax1.set_ylabel(u'espaço(milimetros) ', color='black', fontsize='20')

plt.plot(x1,y1,linestyle='-', color='green',marker='o', linewidth=3.0)

plt.grid(True)
plt.show()
