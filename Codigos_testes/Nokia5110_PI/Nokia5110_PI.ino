#include <SoftwareSerial.h>       //Biblioteca usada com bluetooth
#include <LCD5110_Basic.h>
//#include <LCD5110_Graph.h>
#include <MPU6050.h>              //Biblioteca para o modulo giroscopio/acelerômetro.
#define CLK 13
#define DIN 12
#define DC 11
#define RST 9
#define CE 10
#define SENSOR1 4       //Primeiro sensor.
#define SENSOR2 5
#define SENSOR3 6
#define SENSOR4 7
#define SENSOR5 8       //Ultimo sensor.
#define S_TX 2          //Tx para bluetooth.
#define S_RX 3          //Rx para bluetooth.

SoftwareSerial bluetooth(S_RX, S_TX);
LCD5110 tela(CLK, DIN, DC, RST, CE);

//Obtendo as fontes
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2 , T3, T4;                                //Tempos entre sensores.
int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;                                          //Instancia da classe usada na leitura do acelerometro/giroscopio.

void setup()
{
  Serial.begin (115200);//Comunicação serial configurada.
  bluetooth.begin(9600);//Porta para bluetooth.
  tela.InitLCD(); //Inicializando o display
  tela.setFont(SmallFont); //Definindo a fonte
  pinMode(SENSOR1, INPUT_PULLUP); //Botão.
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    tela.print("MPU",CENTER, 8 * 1);
    tela.print("Disconectado",CENTER, 8 * 2);
    delay(500);
    tela.clrScr();
  }
  delay(2000);//Espera um tempo para se conectar no WiFi
  tela.clrScr(); //Apaga o contéudo que estiver na tela
}

void loop()
{
//  delay(2000); // aguarda dois segundos
  //Mensagem padrão no lcd.
//  tela.print("Trilho Honz", CENTER, 8 * 0);
//  tela.print("Aguardando", CENTER, 8 * 2);
//  tela.print("Valores", CENTER, 8 * 3);
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) { //Momento que o corpo entra no raio de cobertura do sensor
    tela.print("Trilho Honz", CENTER, 8 * 0);
    tela.print("Angulo",CENTER, 8 * 2);
    tela.print(String(getAngleMPU()),CENTER, 8 * 3);
    tela.clrScr();
  }
  while (digitalRead(SENSOR1) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t0 = millis(); // captura o tempo corrente em t0
  tela.clrScr();
  tela.print("Sensor 1", LEFT, 8 * 0);

  //SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR2) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t1 = millis(); // captura o tempo corrente em t1
  tela.print("Sensor 2", LEFT, 8 * 1);

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH);
  while (digitalRead(SENSOR3) == LOW);
  t2 = millis(); // captura o tempo corrente em t2
  tela.print("Sensor 3", LEFT, 8 * 2);

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH);
  while (digitalRead(SENSOR4) == LOW);
  t3 = millis(); // captura o tempo corrente em t3
  tela.print("Sensor 4", LEFT, 8 * 3);

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH);
  while (digitalRead(SENSOR5) == LOW);
  t4 = millis(); // captura o tempo corrente em t4
  tela.print("Sensor 1", LEFT, 8 * 4);

  // Calculo -------------------------------------------
  T1 = (t1 - t0) ;
  T2 = (t2 - t0) ;
  T3 = (t3 - t0) ;
  T4 = (t4 - t0) ;

  tela.clrScr();
  tela.print(F("Dados coletados!"), CENTER, 8 * 2);
  delay(2000);
  tela.clrScr(); //Apaga o contéudo que estiver na tela
  //  tela.invert(true);
  tela.print("Angulo:"+String(getAngleMPU()), LEFT, 8 * 0);
  tela.print("--------------", LEFT, 8 * 1);
  tela.print("T1:" + String(T1), LEFT, 8 * 2);
  tela.print("T2:" + String(T2), LEFT, 8 * 3);
  tela.print("T3:" + String(T3), LEFT, 8 * 4);
  tela.print("T4:" + String(T4), LEFT, 8 * 5);
  //Dados enviados por bluetooth
  bluetooth.print(F("Tempo1|"));
  bluetooth.print(T1);
  delay(1000);
  bluetooth.print(F("Tempo2|"));
  bluetooth.print(T2);
  delay(1000);
  bluetooth.print(F("Tempo3|"));
  bluetooth.print(T3);
  delay(1000);
  bluetooth.print(F("Tempo4|"));
  bluetooth.print(T4);
  delay(1000);
  bluetooth.print(F("Angulo|"));
  bluetooth.print(angulo);
  while (1);
}

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
