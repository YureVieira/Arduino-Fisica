/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 04/04/2018
  VERSÃO: 2.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento TRILHO HORIZONTAL ou QUEDA LIVRE.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em um trilho (perfil em U) ou em queda livre.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  Dados salvos em cartão micro SD.
  É necessário introduzir no Monitor Serial do Arduino a distância entre os sensores.

  --------------------------------------------------------------------------*/
#include <SPI.h>
#include <SD.h>

//Pinos para sensores(Entradas analogicas são usadas como entradas digitais simples).
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2 , T3, T4;                         //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
float distancia = 210.0;                               //Distancia entre sensores.
File myFile;                                          //Instancia da classe usada na escrita/leitura do cartão micro SD.

void setup() {
  //Configuração de pinos como entradas digitais.
  pinMode (SENSOR1, INPUT);
  pinMode (SENSOR2, INPUT);
  pinMode (SENSOR3, INPUT);
  pinMode (SENSOR4, INPUT);
  pinMode (SENSOR5, INPUT);
  //Comunicação serial configurada.
  Serial.begin (115200);
  //Verificação do modulo SD card(Pino 4 é usado).
  if (SD.begin(4))
  {
    Serial.println("Cartão SD pronto para uso.");
  } else
  {
    Serial.println("Falha na inicialização do cartão SD");
    return;
  }
}

// Laco principal do programa
void loop() {
  delay(2000); // aguarda dois segundos
  Serial.println("[START]");
  
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR1) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t0 = millis(); // captura o tempo corrente em t0

  //SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR2) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t1 = millis(); // captura o tempo corrente em t1

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH);
  while (digitalRead(SENSOR3) == LOW);
  t2 = millis(); // captura o tempo corrente em t2

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH);
  while (digitalRead(SENSOR4) == LOW);
  t3 = millis(); // captura o tempo corrente em t3

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH);
  while (digitalRead(SENSOR5) == LOW);
  t4 = millis(); // captura o tempo corrente em t4
  //Após a leitura de todos os sensores, calcula os tempos e velocidades.
  // Calculo -------------------------------------------
  T1 = (t1 - t0) ;
  T2 = (t2 - t0) ;
  T3 = (t3 - t0) ;
  T4 = (t4 - t0) ;

  //Calculo da Velocidade.
  v1 = distancia / T1;
  v2 = 2 * distancia / T2;
  v3 = 3 * distancia / T3;
  v4 = 4 * distancia / T4;

  //Escrita no SD card
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("********Tempos(ms)******");
    myFile.print("T1: ");
    myFile.println(T1);
    myFile.print("T2: ");
    myFile.println(T2);
    myFile.print("T3: ");
    myFile.println(T3);
    myFile.print("T4: ");
    myFile.println(T4);
    myFile.println("******Velocidades(m/s)*****");
    myFile.print("v1: ");
    myFile.println(v1);
    myFile.print("v2: ");
    myFile.println(v2);
    myFile.print("v3: ");
    myFile.println(v3);
    myFile.print("v4: ");
    myFile.println(v4);
    myFile.println("************************");
    myFile.close();
  }
  //  Serial.print("Tempo1|");
  //delay(10000);
  Serial.println(T1);
  //  delay(1000);
  //  Serial.print("Tempo2|");
  Serial.println(T2);
  //  delay(1000);
  //  Serial.print("Tempo3|");
  Serial.println(T3);
  //  delay(1000);
  //  Serial.print("Tempo4|");
  Serial.println(T4);
  //  delay(1000);
  //         Serial.print("v1: ");
  //         Serial.print(v1, 6);
  //         Serial.print("v2: ");
  //         Serial.print(v2, 6);
  //         Serial.print("v3: ");
  //         Serial.print(v3, 6);
  //         Serial.print("v4: ");
  //         Serial.print(v4, 6);
  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  while (1); // loop infinito. reset o arduino para um novo experimento.
}
