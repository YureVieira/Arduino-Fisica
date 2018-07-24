#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define SSID "ESP12E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN D6
#define SCL_PIN D3
#define pin D1

SSD1306Wire  display(0x3c, D6, D5);
// SH1106 display(0x3c, D3, D5);
int count = 0;
char* topic = "/pendulo";
char* server = "10.0.40.168";
boolean wifi = false, mqtt = false;
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
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Buscando conexão");
  wifi = wifiSetup();
  if (wifi)mqtt = mqttSetup();
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  pinMode(pin, INPUT_PULLUP);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema Pronto!");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (wifi) {
    ArduinoOTA.begin();
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
  }
}
/************************************************************************************
** LOOP
************************************************************************************/
void loop() {
  uint32_t time1 = 0, time2 = 0, time3 = 0;
  if (sensor()) {
    time1 = millis();
    while (sensor()) {
      if (wifi)ArduinoOTA.handle();
      yield(); ESP.wdtFeed();
    }
    while (!sensor() || time2 == 0) {
      if (wifi)ArduinoOTA.handle();
      time2 = millis();
      yield(); ESP.wdtFeed();
    }
    while (sensor()) {
      if (wifi)ArduinoOTA.handle();
      yield(); ESP.wdtFeed();
    }
    while (!sensor() || time3 == 0) {
      time3 = millis();
      if (wifi)ArduinoOTA.handle();
      yield(); ESP.wdtFeed();
    }
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
  }
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
  WiFiManager WFManager;
  if (!WFManager.autoConnect(SSID, PASSWRORD)) {
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    //    ESP.reset();
    return false;
  }
  return true;
}
/************************************************************************************
** FUNÇÕES P/ OLED
************************************************************************************/
void oledSetup() {
  display.init();
  display.flipScreenVertically();
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
