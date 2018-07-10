#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define SSID "ESP12E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN D4
#define SCL_PIN D3
#define pin D1

void callback(char* topic, byte * payload, unsigned int length) {
  // handle message arrived
}

SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
bool status;
int count = 0;
char* topic = "/pendulo";
char* server = "10.0.40.159";
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
  Serial.begin(115200);
  wifiSetup();
  oledSetup();
  mqttSetup();
  pinMode(pin, INPUT);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema Pronto!");
  client.publish(topic,"0");
}

void loop() {
  uint32_t time1=0, time2=0, time3=0;
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
    String payload = String(time3 - time1);
    oledNewMessage(display.getWidth() / 2, display.getHeight() / 2 - 16, String(count) + "º ciclo");
    oledAddMessage(display.getWidth() / 2, display.getHeight() / 2, "Periodo: " + payload + " ms");
    Serial.print(String(count) + "º ciclo / ");
    Serial.println("Periodo: " + payload + " ms");
    if (client.connected()) {
      if (client.publish(topic, payload.c_str())) {
        client.publish("/time1", String(time1).c_str());
        client.publish("/time2", String(time2).c_str());
        client.publish("/time3", String(time3).c_str());
        client.publish("/ciclo", String(count).c_str());
        Serial.println("Publish ok");
      }
      else {
        Serial.println("Publish failed");
      }
    }
    else {
      Serial.println("Disconectado");
      client.connect("ESP8266_872ysrjo21387y");
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
void mqttSetup() {
  if (client.connect("ESP8266_872ysrjo21387y"))
    Serial.println("Connected to MQTT broker");
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}
/****************** FUNÇÕES P/ OLED ***********************************/
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
