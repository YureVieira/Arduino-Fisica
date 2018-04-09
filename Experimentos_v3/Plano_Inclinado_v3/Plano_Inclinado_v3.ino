/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 09/04/2018
  VERSÃO: 3.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento PLANO INCLINADO.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em um trilho (perfil em U) e um modulo MPU6050
  (acelerometro/giroscopio) para aferir o grau de inclinação.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  Dados são mostrados em um modulo lcd 16x2 i2c.
  É necessário introduzir no Monitor Serial do Arduino a distância entre os sensores.

  --------------------------------------------------------------------------*/
#include <Wire.h>
#include <MPU6050.h>              //Biblioteca para o modulo giroscopio/acelerômetro.
#include <LiquidCrystal_I2C.h>    //Biblioteca para o modulo LCD-i2c.
//Pinos para sensores(entradas analogicas)
#define SENSOR1 4
#define SENSOR2 5
#define SENSOR3 6
#define SENSOR4 7
#define SENSOR5 8

// Inicializa o display no endereco 0x3f
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2 , T3, T4;                         //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
float distancia = 210.0;
int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;                                          //Instancia da classe usada na leitura do acelerometro/giroscopio.

/*****************************************************/
//Função que retorna o angulo de inclinação
int getAngleMPU() {
  //valores de correção obtido impiricamente
  int offset = -90;
  int factor = 1;
  int value;
  // Leitura dos valores normalizados
  Vector normAccel = mpu.readNormalizeAccel();

  // Calcula Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;
  value = factor * roll + offset;
  return value;
}

void LCD_NovaMensagem(String str,int i=0,int j=0){
  lcd.clear();
  lcd.setCursor(i, j);
  lcd.print(str);
  }
  void LCD_Mensagem(String str,int i=0,int j=0){
  lcd.setCursor(i, j);
  lcd.print(str);
  }
  
/*****************************************************/
void setup() {
  //Configuração de pinos como entradas digitais.
  pinMode (SENSOR1, INPUT);
  pinMode (SENSOR2, INPUT);
  pinMode (SENSOR3, INPUT);
  pinMode (SENSOR4, INPUT);
  pinMode (SENSOR5, INPUT);
  lcd.begin (16, 2);
  lcd.setBacklight(HIGH);
  //Comunicação serial configurada.
  Serial.begin (115200);
  LCD_NovaMensagem("MPU6050 ...");
  //Tentativa de conexão com modulo MPU6050.
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    LCD_NovaMensagem("MPU disconectado");
    delay(500);
    return;
  }
}

/*****************************************************/
// Laco principal do programa
void loop() {
  delay(2000); // aguarda dois segundos
  Serial.println("[START]");
  do{
  LCD_NovaMensagem("Plano inclinado");
  LCD_Mensagem("Angulo: "0,1);//0->primeira coluna; 1->segunda linha
  LCD_Mensagem(String(getAngleMPU()),8,1);//8->nona coluna; 1->segunda linha
  }while(digitalRead(SENSOR1) == LOW)
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

  //Leitura do angulo
  angulo = getAngleMPU();

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

