#include "U8glib.h"
// A linha abaixo define as ligacoes e deve ser
// ajustada conforme o display utilizado.
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 , 7); //Enable, RW, RS, RESET
//Pinos para sensores(Entradas analogicas são saudas como entradas digitais simples)
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
unsigned long T1 = 0, T2 = 0, T3 = 0, T4 = 0;
float v1, v2, v3, v4;
float distancia;
int sensor_atual=5;
int leitura;

void u8g_prepare()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}
//# rotinas para LCD
void LCD_printtempos() {
  String tempo1 = String(T1);
  String tempo2 = String(T2);
  String tempo3 = String(T3);
  String tempo4 = String(T4);
  String veloc1 = String(v1);
  String veloc2 = String(v2);
  String veloc3 = String(v3);
  String veloc4 = String(v4);

  char msg1[23], msg2[23], msg3[23], msg4[23];

  sprintf(msg1, "T1=%s", tempo1.c_str());
  sprintf(msg2, "T2=%s", tempo2.c_str());
  sprintf(msg3, "T3=%s", tempo3.c_str());
  sprintf(msg4, "T4=%s",tempo4.c_str());

  u8g.drawStr( 0, 0, "Tempos x velocidades");
  
  u8g.drawStr( 0, 12, msg1);
  u8g.drawStr( 0, 22, msg2);
  u8g.drawStr( 0, 32, msg3);
  u8g.drawStr( 0, 42, msg4);

  sprintf(msg1, "v1=%s", veloc1.c_str());
  sprintf(msg2, "v2=%s", veloc2.c_str());
  sprintf(msg3, "v3=%s", veloc3.c_str());
  sprintf(msg4, "v4=%s", veloc4.c_str());

  u8g.drawStr( 64, 12, msg1);
  u8g.drawStr( 64, 22, msg2);
  u8g.drawStr( 64, 32, msg3);
  u8g.drawStr( 64, 42, msg4);

  u8g.drawStr( 0, 55, " (ms)      (m/s) ");  
}

void draw() {
  u8g_prepare();
  LCD_printtempos();
}

void setup() {
  pinMode (S1, INPUT);//INPUT(tests)
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  Serial.begin (9600);
  Serial.println("Entre com a distancia em milimetros entre os sensores(1 valor)");
  String distancia_str = "";
  while (1)
  {
    while (Serial.available()) {
      char dado = Serial.read();
      distancia_str.concat(dado);
    }
    distancia = distancia_str.toFloat();
    if (distancia > 0) {
      Serial.print("Distancia entre sensores configurada para: ");
      Serial.print(distancia);
      Serial.println(" mm");
      //distancia = distancia / 1000;
      break;
    }
    else {
      Serial.println("Entre com um valor valido!");
      distancia_str = "";
    }
    delay(1000);
  }
  u8g.setRot180();//Flip do texto no lcd(se necessario)
  u8g_prepare();
  u8g.firstPage();
  do {
    u8g.drawStr( 4, 0, "## Sistema Pronto ##");
  } while ( u8g.nextPage() );
}
// Laco principal do programa
void loop() {
  if (distancia > 0.0)
  {
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
    //Pos leitura
    if ( leitura == 1 ) {
      T1 = (t2 - t1); // 1000.0;
      T2 = (t3 - t1); // 1000.0;
      T3 = (t4 - t1); // 1000.0;
      T4 = (t5 - t1); // 1000.0;
      
      //Calculo da Velocidade
      v1 = distancia / T1;
      v2 = 2*distancia / T2;
      v3 = 3*distancia / T3;
      v4 = 4*distancia / T4;
      
      leitura = 0;
      Serial.println("############ TEMPOS ################ ");
      Serial.print(T1, 6);
      Serial.println(" ms");
      Serial.print(T2, 6);
      Serial.println(" ms");
      Serial.print(T3, 6);
      Serial.println(" ms");
      Serial.print(T4, 6);
      Serial.println("########### VELOCIDADES ############ ");
      Serial.print(v1, 6);
      Serial.println(" m/s");
      Serial.print(v2, 6);
      Serial.println(" m/s");
      Serial.print(v3, 6);
      Serial.println(" m/s");
      Serial.print(v4, 6);
      Serial.println(" m/s");
      Serial.println("#################################### ");
      //Escrita no LCD
      u8g.firstPage();
      do
      {
        draw();
      }
      while ( u8g.nextPage() );
      delay(1000);
    }
  }
}
