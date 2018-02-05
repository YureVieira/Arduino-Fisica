#include "U8glib.h"
#include <SPI.h>
#include <SD.h>


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
float T1 = 0, T2 = 0, T3 = 0, T4 = 0;
int current_sensor;
int full_reading;
File myFile;

void u8g_prepare()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void u8g_setup() {
}

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

  u8g.drawStr( 2, 0, "# Amostras de tempo #");

  u8g.drawStr( 0, 10, msg1);
  u8g.drawStr( 0, 20, msg2);
  u8g.drawStr( 0, 30, msg3);
  u8g.drawStr( 0, 40, msg4);

}

void draw() {
  u8g_prepare();
  LCD_printValues();
}

void setup() {
  pinMode (S1, INPUT);//INPUT(tests)
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  Serial.begin (9600);
  current_sensor = 5;
  u8g.setRot180();//Flip do texto no lcd(se necessario)
  u8g_prepare();
  u8g.firstPage();
  u8g.drawStr( 4, 0, "## Sistema Pronto ##");
  //SD card
  if (SD.begin(4))
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }

}
// Laco principal do programa
void loop() {
  if (digitalRead(S1) == LOW && current_sensor == 5) {
    t1 = micros();
    current_sensor = 1;
    Serial.println("Sensor1");
  }
  if (digitalRead(S2) == LOW && current_sensor == 1) {
    t2 = micros();
    current_sensor = 2;
    Serial.println("Sensor2");
  }
  if (digitalRead(S3) == LOW && current_sensor == 2) {
    t3 = micros();
    current_sensor = 3;
    Serial.println("Sensor3");
  }
  if (digitalRead(S4) == LOW && current_sensor == 3) {
    t4 = micros();
    current_sensor = 4;
    Serial.println("Sensor4");
  }
  if (digitalRead(S5) == LOW && current_sensor == 4) {
    t5 = micros();
    current_sensor = 5;
    Serial.println("Sensor5");
    full_reading = 1;
  }
  //Pos leitura
  if ( full_reading == 1 ) {
    T1 = (t2 - t1) / 1000.0;
    T2 = (t3 - t2) / 1000.0;
    T3 = (t4 - t3) / 1000.0;
    T4 = (t5 - t4) / 1000.0;
    full_reading = 0;
    Serial.println("############ LEITURA ############### ");
    Serial.println(T1, 3);
    Serial.println(T2, 3);
    Serial.println(T3, 3);
    Serial.println(T4, 3);
    Serial.println("#################################### ");
    //Escrita no SD card
    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      myFile.println("************************");
      myFile.print("T1: ");
      myFile.println(T1);
      myFile.print("T2: ");
      myFile.println(T2);
      myFile.print("T3: ");
      myFile.println(T3);
      myFile.print("T4: ");
      myFile.println(T4);
      myFile.println("************************");
      myFile.close(); // close the file
    }
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
