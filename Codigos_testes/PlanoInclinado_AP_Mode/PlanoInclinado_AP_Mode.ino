#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <MPU6050.h>              //Biblioteca para o modulo giroscopio/acelerômetro.
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define SENSOR1 D1       //Primeiro sensor.
#define SENSOR2 D2
#define SENSOR3 D5
#define SENSOR4 D6
#define SENSOR5 D7       //Ultimo sensor.
#define SDA_PIN D4
#define SCL_PIN D3
unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
//float T1, T2 , T3, T4;                         //Tempos entre sensores.
//float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
//float distancia = 210.0;
int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;                                          //Instancia da classe usada na leitura do acelerometro/giroscopio.
SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
ESP8266WebServer server(80);//webserver
const char* ssid = "PlanoInclinado";
const char* password = "plano123";
String main_msg;
const char* prefix = "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"2\"><meta charset=\"utf-8\"><title>Plano Inclinado</title><style type=\"text/css\"> body{ font-family: Helvetica, sans-serif; } table,th,td{text-align: center; width: 20%; border: 1px solid black; } </style></head><body><h1>Plano Inclinado</h1><table>";
const char* sufix = "</table></body></html>";

/************************************************************************************/
void setup()
{
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(0, 0, "[MPU]: Inicializando MPU6050");

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, 0x68, SDA_PIN, SCL_PIN))
  {
    oledNewMessage(0, 0, "[MPU]: MPU6050 não encontrado!");
    delay(500);
  }
  WiFi.softAP(ssid,password);
  // Align text vertical/horizontal center
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "IP: 192.168.4.1");
  pinMode(SENSOR1, INPUT_PULLUP); //Botão.
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  delay(2000);//Espera um tempo para se conectar no WiFi
}
///////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Iniciando em 2s...");
  delay(2000); // aguarda dois segundos
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) {
    server.handleClient();
    oledNewMessage(display.getWidth() / 2, 10, "Clientes: "+String(WiFi.softAPgetStationNum())+" Inclinação: "+String(getAngleMPU())+"°"+" IP: 192.168.4.1");
    yield();//Efetua o feed do SW WDT
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR1) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t0 = millis(); // captura o tempo corrente em t0
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  oledNewMessage(0, 0, "t0 = " + String(t0));

  // SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR2) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t1 = millis(); // captura o tempo corrente em t1
  oledAddMessage(0, 11, "t1 = " + String(t1));

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR3) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t2 = millis(); // captura o tempo corrente em t2
  oledAddMessage(0, 22, "t2 = " + String(t2));

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR4) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t3 = millis(); // captura o tempo corrente em t3
  oledAddMessage(0, 33, "t3 = " + String(t3));

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR5) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t4 = millis(); // captura o tempo corrente em t4
  oledAddMessage(0, 44, "t4 = " + String(t4));

  // Calculo -------------------------------------------
  //  Serial.print("Tempo decorrido 2 (ms): ");
  //  Serial.println(t1 - t0);
  //  Serial.print("Tempo decorrido 3 (ms): ");
  //  Serial.println(t2 - t0);
  //  Serial.print("Tempo decorrido 4 (ms): ");
  //  Serial.println(t3 - t0);
  //  Serial.print("Tempo decorrido 5 (ms): ");
  //  Serial.println(t4 - t0);
  int angle = getAngleMPU();
  oledNewMessage(0, 0, "Tempo 1 (ms): " + String(t1 - t0));
  oledAddMessage(0, 0, "\nTempo 2 (ms): " + String(t2 - t0));
  oledAddMessage(0, 0, "\n\nTempo 3 (ms): " + String(t3 - t0));
  oledAddMessage(0, 0, "\n\n\nTempo 4 (ms): " + String(t4 - t0));
  oledAddMessage(0, 0, "\n\n\n\nInclinação ( °): " + String(angle));

  main_msg = "<tr><td><b>Leitura 1(ms):</b></td><td>" ;
  main_msg += String(t1-t0)+"</td></tr>";
  main_msg += "<tr><td><b>Leitura 2(ms)</b></td><td>";
  main_msg += String(t2-t0)+"</td></tr>";
  main_msg += "<tr><td><b>Leitura 3(ms)</b></td><td>";
  main_msg += String(t3-t0)+"</td></tr>";
  main_msg += "<tr><td><b>Leitura 4(ms)</b></td><td>";
  main_msg += String(t4-t0)+"</td></tr>";
  main_msg += "<tr><td><b>Inclinação ( º): </b></td><td>";
  main_msg += String(angle)+"</td></tr>";

  long counter = millis();
  //  while (millis() - counter < 60000) {
  //    ESP.wdtFeed();
  //  }
  while (1) {
    server.handleClient();
    ESP.wdtFeed();
  }
}
/****************** FUNÇÕES P/ OLED ***********************************/
void oledNewMessage(int16_t x, int16_t y, String msg) {
  display.clear();
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
void oledAddMessage(int16_t x, int16_t y, String msg) {
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
/****************** FUNÇÕES MPU6050 ***********************************/
//Função que retorna o angulo de inclinação
int getAngleMPU() {
  //valores de correção obtido impiricamente
  int offset = -91;
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
/**************** FUNÇÕES DO WEBSERVER *****************************/
void handleRoot() {
  String page = prefix + main_msg +sufix;
  server.send(200, "text/html", page);
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
