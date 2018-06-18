/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 05/03/2018
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
  Dados mostrados em um display LCD 128x64
  A introdução no Monitor Serial do Arduino a distância entre os sensores é opcional.

  --------------------------------------------------------------------------*/
#include "U8glib.h"
// A linha abaixo define as ligacoes e deve ser ajustada conforme o display utilizado.
U8GLIB_ST7920_128X64_4X u8g(10);//Ligação confirme https://circuitdigest.com/fullimage?i=circuitdiagram_mic/Interfacing-circuit-diagram-of-Graphical-LCD-with-Arduino.png
//Pinos para sensores(Entradas analogicas são usadas como entradas digitais simples)
#define S1 5
#define S2 6
#define S3 7
#define S4 8
#define S5 9

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2, T3, T4;                                 //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
int sensor_atual = 5;                                 //Sensor ativado atualmente, importante para manter a sequencia de leitura de sensores.
int leitura;                                          //Habilita o calculo de velocidade após o termino da leitura de todos os sensores.
float distancia = -1.0;                               //Distancia entre sensores.

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
  String value1 = String(T1);
  String value2 = String(T2);
  String value3 = String(T3);
  String value4 = String(T4);

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

  value1 = String(v1);
  value2 = String(v2);
  value3 = String(v3);
  value4 = String(v4);

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
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  //Comunicação serial configurada.
  Serial.begin (9600);
  
  //Serial.print("Entre com a distancia em milimetros entre os sensores(1 valor)");
  String distancia_str;
 //Laço para colher dados a referentes da distancia entre sensores.
  //Apos 30 segundos atribuir um valor padrão de 200 mm.
  while (millis()<10000) {
    while (Serial.available()) {
      char dado = Serial.read();
      distancia_str.concat(dado);
    }

    //Conversão de string para float(texto p/ numero decimal)
    distancia = distancia_str.toFloat();

    //Quando a distancia for maior que zero, saia do laço.
    if (distancia > 0) {
      //Serial.print("Distancia entre sensores configurada para: ");
      //Serial.print(distancia);
      //Serial.print(" mm/");
      break;
    }
    else {
      //Serial.print("Entre com um valor valido!/");
      distancia_str = "";
    }
    delay(1000);
  }
  if(distancia <= 0){
    distancia = 210;
    }
  //Serial.print("Sistema pronto/");
  
  u8g.setRot180();//Flip do texto no lcd(se necessario)
  u8g_prepare();
  u8g.firstPage();
  do
  {
    //Mensagem no display.
    u8g.drawStr( 4, 0, "## Sistema Pronto ##");
  }
  while ( u8g.nextPage() );
}

// Laco principal do programa
void loop() {
  if (digitalRead(S1) == LOW && sensor_atual == 5) {
    t1 = millis();
    sensor_atual = 1;
    //Serial.print("Sensor1/");
  }
  if (digitalRead(S2) == LOW && sensor_atual == 1) {
    t2 = millis();
    sensor_atual = 2;
    //Serial.print("Sensor2/");
  }
  if (digitalRead(S3) == LOW && sensor_atual == 2) {
    t3 = millis();
    sensor_atual = 3;
    //Serial.print("Sensor3/");
  }
  if (digitalRead(S4) == LOW && sensor_atual == 3) {
    t4 = millis();
    sensor_atual = 4;
    //Serial.print("Sensor4/");
  }
  if (digitalRead(S5) == LOW && sensor_atual == 4) {
    t5 = millis();
    sensor_atual = 5;
    //Serial.print("Sensor5/");
    leitura = 1;
  }
  //Pos leitura
  if ( leitura == 1 ) {
    T1 = (t2 - t1) ;
    T2 = (t3 - t1) ;
    T3 = (t4 - t1) ;
    T4 = (t5 - t1) ;

    //Calculo da Velocidade
    v1 = distancia / T1;
    v2 = 2 * distancia / T2;
    v3 = 3 * distancia / T3;
    v4 = 4 * distancia / T4;
    leitura = 0;

    Serial.print("Tempo1|");
    Serial.print(T1);
    delay(1000);
    Serial.print("Tempo2|");
    Serial.print(T2);
    delay(1000);
    Serial.print("Tempo3|");
    Serial.print(T3);
    delay(1000);
    Serial.print("Tempo4|");
    Serial.print(T4);
    delay(1000);
    Serial.print("v1|");
    Serial.print(v1, 6);
    delay(1000);
    Serial.print("v2|");
    Serial.print(v2, 6);
    delay(1000);
    Serial.print("v3|");
    Serial.print(v3, 6);
    delay(1000);
    Serial.print("v4|");
    Serial.print(v4, 6);

    //Escrita no LCD
    u8g.firstPage();
    do
    {
      //Escrita dos dados no display de LCD.
      draw();
    }
    while ( u8g.nextPage() );
    delay(1000);
  }
}
