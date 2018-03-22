/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 22/03/2018
  VERSÃO: 1.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento QUEDA LIVRE.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em queda livre.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  É necessário introduzir no Monitor Serial do Arduino a distância entre os sensores.

  --------------------------------------------------------------------------*/

const int sensor1 = A0;
const int sensor2 = A1;
const int sensor3 = A2;
const int sensor4 = A3;
const int sensor5 = A4;
const int Rele = A5;

//const int pinTone = 8;

unsigned long t0; // tempo inicial
unsigned long t1; // tempo final no sensor2
unsigned long t2; // tempo final no sensor3
unsigned long t3; // tempo final no sensor4
unsigned long t4; // tempo final no sensor5

void setup()
{
  Serial.begin(9600);
  pinMode(sensor1, INPUT_PULLUP); //Botão
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(rele, OUTPUT);
  //pinMode(pinTone, OUTPUT);
}

void loop()
{
  //Serial.print("Iniciando em 2s...");
  delay(2000); // aguarda dois segundos

  // sensor1 -------------------------------------------
  Serial.println("Aguardando a resposta do sensor1...");
  if(digitalRead(sensor1) == HIGH) return;

  t0 = millis(); // captura o tempo corrente em t0
  Serial.print("t0 (ms) = "); Serial.println(t0);
  //tone(pinTone, 800,50);
  // sensor2 -------------------------------------------
  Serial.println("Aguardando a resposta do sensor2...");
  while (digitalRead(sensor2) == HIGH);
  while (digitalRead(sensor2) == LOW);

  t1 = millis(); // captura o tempo corrente em t1
  Serial.print("t1 (ms) = "); Serial.println(t1);
  //tone(pinTone, 800,50);
  //sensor3---------------------------------------------
  Serial.println("Aguardando a resposta do sensor3...");
  while (digitalRead(sensor3) == HIGH);
  while (digitalRead(sensor3) == LOW);

  t2 = millis(); // captura o tempo corrente em t2
  Serial.print("t2 (ms) = "); Serial.println(t2);
  //tone(pinTone, 800,50);
  //sensor4---------------------------------------------
  Serial.println("Aguardando a resposta do sensor4...");
  while (digitalRead(sensor4) == HIGH);
  while (digitalRead(sensor4) == LOW);

  t3 = millis(); // captura o tempo corrente em t3
  Serial.print("t3 (ms) = "); Serial.println(t3);
  //tone(pinTone, 800,50);
  //sensor5---------------------------------------------
  Serial.println("Aguardando a resposta do sensor5...");
  while (digitalRead(sensor5) == HIGH);
  while (digitalRead(sensor5) == LOW);

  t4 = millis(); // captura o tempo corrente em t4
  Serial.print("t4 (ms) = "); Serial.println(t4);
  //tone(pinTone, 800,50);




  // Calculo -------------------------------------------
  //Serial.print("Tempo decorrido 1_2 (ms): ");
  Serial.println(t1 - t0);
  //Serial.print("Tempo decorrido 1_3 (ms): ");
  Serial.println(t2 - t0);
  //Serial.print("Tempo decorrido 1_4 (ms): ");
  Serial.println(t3 - t0);
  //Serial.print("Tempo decorrido 1_5 (ms): ");
  Serial.println(t4 - t0);

  delay(1000);
  //notone(pinTone);
  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  while (1); // loop infinito. reset o arduino para um novo experimento.
}
