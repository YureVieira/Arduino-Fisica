//#define WFM_ENABLE
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define SSID "ESP32E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN 21
#define SCL_PIN 22
#define pin 23

SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
// SH1106 display(0x3c, D3, D5);
int count = 0;
int _delay = 300;
char* topic = "/pendulo";
char* server = "10.0.40.144";
bool wifi = false, mqtt = false;
WiFiClient wifiClient;
void callback(char* topic, byte * payload, unsigned int length);
PubSubClient client(server, 1883, callback, wifiClient);

/************************************************************************************
** SETUP
************************************************************************************/
void setup() {
  oledSetup();
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Buscando conexao");
  //tenta se conectar com uma rede wifi
  wifi = wifiSetup();
  //Se esta conectado tente se conectar ao broker mqtt
  if (wifi)mqtt = mqttSetup();
  pinMode(pin, INPUT_PULLUP);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema Pronto!");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  ArduinoOTA.setHostname("Pendulo_ESP32");
  if (wifi) {
    ArduinoOTA.onStart([]() {
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 10, "OTA Update");
      display.display();
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
      display.display();
    });

    ArduinoOTA.onEnd([]() {
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Restart");
      display.display();
    });
    ArduinoOTA.begin();
  }
}
/************************************************************************************
** LOOP
************************************************************************************/
void loop() {
  uint32_t time1 = 0, time2 = 0, time3 = 0;
//  if (sensor()) {
    while (!sensor()) {
      if (wifi)ArduinoOTA.handle();
      yield();
    }
    time1 = millis();
    delay(_delay);
    while (!sensor()) {
      if (wifi)ArduinoOTA.handle();
      yield();
    }
    time2 = millis();
    delay(_delay);
    while (!sensor()) {
      if (wifi)ArduinoOTA.handle();
      yield();
    }
  time3 = millis();
    count++;
    String payload = String(time3 - time1);
    oledNewMessage(0, 0, "T1: " + String(time1));
    oledAddMessage(0, 10, "T2: " + String(time2));
    oledAddMessage(0, 20, "T3: " + String(time3));
    oledAddMessage(0, 30, "Periodo: " + String(time3 - time1));
    oledAddMessage(0, 40, "Payload: " + payload);
    Serial.print(String(count) + "º ciclo / ");
    Serial.println("Periodo: " + payload + " ms");
    if (wifi && mqtt) {
      if (client.connected()) {
        if (client.publish(topic, payload.c_str())) {
          oledAddMessage(0, 50, "Publish ok");
        }
        else {
          oledAddMessage(0, 50, "Publish failed");
        }
      }
      else {
        oledAddMessage(0, 0, "Diconectado");
        client.connect("ESP8266_872ysrjo21387y");
      }
    }
    if (wifi)ArduinoOTA.handle();
//  }
//  ArduinoOTA.handle();
}

/************************************************************************************
** FUNÇÕES P/ MQTT
************************************************************************************/
void callback(char* topic, byte * payload, unsigned int length) {
  // handle message arrived
}
bool mqttSetup() {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (client.connect("ESP8266_872ysrjo21387y")) {
    oledNewMessage(0, 0, "Connected to MQTT broker");
    delay(1000);
    return true;
  }
  else {
    oledNewMessage(0, 0, "MQTT connect failed");
    //    oledAddMessage(0, 10, "Will reset and try again...");
    delay(5000);
    return false;
    //    abort();
  }
}
/************************************************************************************
** FUNÇÕES P/ GPIO
************************************************************************************/
bool sensor() {
  return !digitalRead(pin);
}
/************************************************************************************
** FUNÇÕES P/ CONEXÃO
************************************************************************************/
bool wifiSetup() {
  const char* ssid = "LME09";
  const char* password = "138027Sn";
#ifdef WFM_ENABLE
  WiFiManager WFManager;
  if (!WFManager.autoConnect(SSID, PASSWRORD)) {
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    //    ESP.reset();
    return false;
  }
#else if
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    oledNewMessage(0, 0, "Connection Failed! Rebooting...");
    //    delay(5000);
    //    ESP.restart();
    return false;
  }
#endif
  return true;
}
/************************************************************************************
** FUNÇÕES P/ OLED
************************************************************************************/
void oledSetup() {
  display.init();
  //  display.flipScreenVertically();
  display.setContrast(255);
}
void oledNewMessage(int16_t x, int16_t y, String msg) {
  display.clear();
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
void oledAddMessage(int16_t x, int16_t y, String msg) {
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
