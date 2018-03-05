/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 28/02/2018
  VERSÃO: 1.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento TRILHO HORIZONTAL ou QUEDA LIVRE.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em um trilho (perfil em U) ou em queda livre.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  É necessário introduzir no Monitor Serial do Arduino a distância entre os sensores.

  --------------------------------------------------------------------------*/

//Pinos para sensores
#define S1 8
#define S2 9
#define S3 10
#define S4 11
#define S5 12

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2, T3, T4;                                 //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
int sensor_atual = 5;                                 //Sensor ativado atualmente, importante para manter a sequencia de leitura de sensores.
int leitura;                                          //Habilita o calculo de velocidade após o termino da leitura de todos os sensores.
float distancia = -1.0;                               //Distancia entre sensores.

void setup() {
  //Configuração de pinos como entradas digitais.
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  //Comunicação serial configurada.
  Serial.begin (9600);
  
  Serial.println("Entre com a distancia em milimetros entre os sensores(1 valor)");
  String distancia_str = "";
  //Laço para colher dados a referentes da distancia entre sensores.
  while (1) {
    while (Serial.available()) {
      char dado = Serial.read();
      distancia_str.concat(dado);
    }

    //Conversão de string para float(texto p/ numero decimal)
    distancia = distancia_str.toFloat();

    //Quando a distancia for maior que zero, saia do laço.
    if (distancia > 0) {
      Serial.print("Distancia entre sensores configurada para: ");
      Serial.print(distancia);
      Serial.println(" mm");
      break;
    }
    else {
      Serial.println("Entre com um valor valido!");
      distancia_str = "";
    }
    delay(1000);
  }
}

// Laco principal do programa
void loop() {
  if (digitalRead(S1) == LOW && sensor_atual == 5) {
    t1 = millis();
    sensor_atual = 1;
    Serial.println("Sensor1");
  }
  if (digitalRead(S2) == LOW && sensor_atual == 1) {
    t2 = millis();
    sensor_atual = 2;
    Serial.println("Sensor2");
  }
  if (digitalRead(S3) == LOW && sensor_atual == 2) {
    t3 = millis();
    sensor_atual = 3;
    Serial.println("Sensor3");
  }
  if (digitalRead(S4) == LOW && sensor_atual == 3) {
    t4 = millis();
    sensor_atual = 4;
    Serial.println("Sensor4");
  }
  if (digitalRead(S5) == LOW && sensor_atual == 4) {
    t5 = millis();
    sensor_atual = 5;
    Serial.println("Sensor5");
    leitura = 1;
  }
  //Após a leitura de todos os sensores, calcula os tempos e velocidades.
  if ( leitura == 1 ) {
    T1 = (t2 - t1) ;
    T2 = (t3 - t1) ;
    T3 = (t4 - t1) ;
    T4 = (t5 - t1) ;

    //Calculo da Velocidade.
    v1 = distancia / T1;
    v2 = 2 * distancia / T2;
    v3 = 3 * distancia / T3;
    v4 = 4 * distancia / T4;

    leitura = 0;
    Serial.println("############ TEMPOS ################ ");
    Serial.print("T1: ");
    Serial.print(T1);
    Serial.println(" ms");
    Serial.print("T2: ");
    Serial.print(T2);
    Serial.println(" ms");
    Serial.print("T3: ");
    Serial.print(T3);
    Serial.println(" ms");
    Serial.print("T4: ");
    Serial.print(T4);
    Serial.println(" ms");
    Serial.println("########### VELOCIDADES ############ ");
    Serial.print("v1: ");
    Serial.print(v1, 6);
    Serial.println(" m/s");
    Serial.print("v2: ");
    Serial.print(v2, 6);
    Serial.println(" m/s");
    Serial.print("v3: ");
    Serial.print(v3, 6);
    Serial.println(" m/s");
    Serial.print("v4: ");
    Serial.print(v4, 6);
    Serial.println(" m/s");
    Serial.println("#################################### ");
    delay(1000);
  }
}
