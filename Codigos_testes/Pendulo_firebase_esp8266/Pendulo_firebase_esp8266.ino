#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <FirebaseArduino.h>
#include <Ticker.h>
#include <Wire.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define FIREBASE_HOST "fisica-5ef2d.firebaseio.com"
#define FIREBASE_AUTH "6QDWXGcLikrjXUQ4xw5yzp0eUeYhjInAM2PpKLbF"
#define PUBLISH_INTERVAL 1000
#define SSID "ESP12E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN D4
#define SCL_PIN D3
#define pin D1

SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
bool status;
int count = 0;


void setup() {
  Serial.begin(115200);
  wifiSetup();
  firebaseSetup();
  oledSetup();
  pinMode(pin, INPUT);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema Pronto!");
}

void loop() {
  uint32_t time1, time2, time3;
  if (sensor()) {
    time1 = millis();
    while (sensor()) {
      yield();
      ESP.wdtFeed();
    }
    while (!sensor()) {
      time2 = millis();
      yield(); ESP.wdtFeed();
    }
    while (sensor()) {
      yield();
      ESP.wdtFeed();
    }
    while (!sensor()) {
      time3 = millis();
      yield(); ESP.wdtFeed();
    }
    count++;
    oledNewMessage(display.getWidth() / 2, display.getHeight() / 2-16, String(count) + "º ciclo");
    oledAddMessage(display.getWidth() / 2, display.getHeight() / 2, "Periodo: " + String(time3 - time1) + " ms");
    Serial.print(String(count) + "º ciclo / ");
    Serial.println("Periodo: " + String(time3 - time1) + " ms");
    Firebase.pushInt("pendulo/periodo", (int)time3 - time1);
    if (Firebase.failed()) {
      Serial.print("failed:");
      Serial.println(Firebase.error());
    }
  }
}

bool sensor() {
  return !digitalRead(pin);
}

void wifiSetup() {
  WiFiManager WFManager;
  if (!WFManager.autoConnect(SSID, PASSWRORD)) {
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
  }
}

void firebaseSetup() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //  Firebase.setBool("lamp", false);
  //  Firebase.setBool("presence", false);
}
/****************** FUNÇÕES P/ OLED ***********************************/
void oledSetup(){
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
