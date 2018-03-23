/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 23/03/2018
  VERSÃO: 3.0
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

#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2, T3, T4;                                 //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
float distancia = 210.0;                               //Distancia entre sensores.

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR1, INPUT_PULLUP); //Botão
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  Serial.println("Aguardando a resposta do SENSOR1...");
}

void loop()
{
  Serial.print("Iniciando em 2s...");
  delay(2000); // aguarda dois segundos

  // SENSOR1 -------------------------------------------
  //Serial.println("Aguardando a resposta do SENSOR1...");
  if (digitalRead(SENSOR1) == HIGH) return;
  t0 = millis(); // captura o tempo corrente em t0
  Serial.print("t0 (ms) = "); Serial.println(t0);

  // SENSOR2 -------------------------------------------
  Serial.println("Aguardando a resposta do SENSOR2...");
  while (digitalRead(SENSOR2) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR2) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t1 = millis(); // captura o tempo corrente em t1
  Serial.print("t1 (ms) = "); Serial.println(t1);

  //SENSOR3---------------------------------------------
  Serial.println("Aguardando a resposta do SENSOR3...");
  while (digitalRead(SENSOR3) == HIGH);
  while (digitalRead(SENSOR3) == LOW);
  t2 = millis(); // captura o tempo corrente em t2
  Serial.print("t2 (ms) = "); Serial.println(t2);

  //SENSOR4---------------------------------------------
  Serial.println("Aguardando a resposta do SENSOR4...");
  while (digitalRead(SENSOR4) == HIGH);
  while (digitalRead(SENSOR4) == LOW);
  t3 = millis(); // captura o tempo corrente em t3
  Serial.print("t3 (ms) = "); Serial.println(t3);

  //SENSOR5---------------------------------------------
  Serial.println("Aguardando a resposta do SENSOR5...");
  while (digitalRead(SENSOR5) == HIGH);
  while (digitalRead(SENSOR5) == LOW);
  t4 = millis(); // captura o tempo corrente em t4
  Serial.print("t4 (ms) = "); Serial.println(t4);

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

  Serial.print(T1);
  Serial.print(T2);
  Serial.print(T3);
  Serial.print(T4);
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
