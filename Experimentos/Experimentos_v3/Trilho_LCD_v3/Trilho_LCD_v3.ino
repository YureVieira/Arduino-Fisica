/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 12/04/2018
  VERSÃO: 3.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento TRILHO HORIZONTAL ou QUEDA LIVRE.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em um trilho (perfil em U) ou em queda livre.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  Dados mostrados em um display LCD 128x64
  É necessário introduzir no Monitor Serial do Arduino a distância entre os sensores.

  --------------------------------------------------------------------------*/
#include "U8glib.h"
#include <SoftwareSerial.h>       //Biblioteca usada com bluetooth
// A linha abaixo define as ligacoes e deve ser ajustada conforme o display utilizado.
U8GLIB_ST7920_128X64_4X u8g(10);//Ligação mostrada na imagem https://circuitdigest.com/fullimage?i=circuitdiagram_mic/Interfacing-circuit-diagram-of-Graphical-LCD-with-Arduino.png

#define SENSOR1 4       //Primeiro sensor.
#define SENSOR2 5
#define SENSOR3 6
#define SENSOR4 7
#define SENSOR5 8       //Ultimo sensor.
//#define BOTAO A0        //Botão para funções.
#define S_TX 2          //Tx para bluetooth.
#define S_RX 3          //Rx para bluetooth.

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2, T3, T4;                                 //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
//int sensor_atual = 5;                                 //Sensor ativado atualmente, importante para manter a sequencia de leitura de sensores.
//int leitura;                                          //Habilita o calculo de velocidade após o termino da leitura de todos os sensores.
float distancia = 210.0;                               //Distancia entre sensores.

//Inicializa a interface serial para bluetooth
SoftwareSerial bluetooth(S_RX, S_TX);
//Preparação do display.
void u8g_prepare()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}
//Rotina para imprimir mensagens.
void LCD_printValues() {
  String value1 = String((uint32_t)T1);
  String value2 = String((uint32_t)T2);
  String value3 = String((uint32_t)T3);
  String value4 = String((uint32_t)T4);

  char msg1[23], msg2[23], msg3[23], msg4[23];

  sprintf(msg1, "T1=%s", value1.c_str());
  sprintf(msg2, "T2=%s", value2.c_str());
  sprintf(msg3, "T3=%s", value3.c_str());
  sprintf(msg4, "T4=%s", value4.c_str());

  u8g.drawStr( 2, 0, "Tempos e velocidade");

  u8g.drawStr( 0, 12, msg1);
  u8g.drawStr( 0, 22, msg2);
  u8g.drawStr( 0, 32, msg3);
  u8g.drawStr( 0, 42, msg4);

  value1 = String(v1,5);
  value2 = String(v2,5);
  value3 = String(v3,5);
  value4 = String(v4,5);

  sprintf(msg1, "v1=%s", value1.c_str());
  sprintf(msg2, "v2=%s", value2.c_str());
  sprintf(msg3, "v3=%s", value3.c_str());
  sprintf(msg4, "v4=%s", value4.c_str());

  u8g.drawStr( 64, 12, msg1);
  u8g.drawStr( 64, 22, msg2);
  u8g.drawStr( 64, 32, msg3);
  u8g.drawStr( 64, 42, msg4);

  u8g.drawStr( 0, 55, " (ms)      (m/s) ");
}

void draw() {
  u8g_prepare();
  LCD_printValues();
}

void setup() {
  //Configuração de pinos como entradas digitais.
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);
  //pinMode(BOTAO, INPUT_PULLUP);//Pullup interno despensa resistor externo.
  Serial.begin (115200);//Comunicação serial configurada.
  bluetooth.begin(9600);//Porta para bluetooth.
  
  u8g.setRot180();//Flip do texto no lcd(se necessario)
  u8g_prepare();
  u8g.firstPage();
  do
  {
    //Mensagem no display.
    u8g.drawStr( 4, 0, F("## Sistema Pronto ##"));
  }
  while ( u8g.nextPage() );
}
// Laco principal do programa
void loop() {
  delay(2000); // aguarda dois segundos

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

//Dados para graficos em python
  Serial.println(T1);
  Serial.println(T2);
  Serial.println(T3);
  Serial.println(T4);

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

  Serial.println(F("Experimento finalizado...."));
  Serial.println(F("Reset o Arduino..."));
  //Escrita no LCD
  u8g.firstPage();
  do
  {
    //Escrita dos dados no display de LCD.
    draw();
  }
  while ( u8g.nextPage() );
  delay(1000);
  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  while (1); // loop infinito. reset o arduino para um novo experimento.
}
