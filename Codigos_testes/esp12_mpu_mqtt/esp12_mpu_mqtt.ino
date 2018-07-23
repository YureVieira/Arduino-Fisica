#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <MPU6050.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define SSID "ESP12E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN D1
#define SCL_PIN D2

MPU6050 mpu;                                          //Instancia da classe usada na leitura do acelerometro/giroscopio.
// SH1106 display(0x3c, D3, D5);
int count = 0;
char* topic = "/acc";
char* server = "10.0.40.168";
WiFiClient wifiClient;
void callback(char* topic, byte * payload, unsigned int length);
PubSubClient client(server, 1883, callback, wifiClient);

/************************************************************************************
** SETUP
************************************************************************************/
void wifiSetup();
void mqttSetup();
void mpuSetup();
void setup() {
  Serial.begin(115200);
  wifiSetup();
  mqttSetup();
  mpuSetup();
  Serial.println("Sistema Pronto!");

  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Update");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("Restart");
  });
}
/************************************************************************************
** LOOP
************************************************************************************/
void loop() {
  int value = getAngleMPU();
  String payload = String(value);
  if (client.connected()) {
    if (client.publish(topic, payload.c_str())) {
      Serial.print("Publish ok / ");
      Serial.println(millis());
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("Diconectado");
    client.connect("ESP8266_872ysrjo21387y");
  }
  ArduinoOTA.handle();
  delay(100);
}

/************************************************************************************
** FUNÇÕES P/ MQTT
************************************************************************************/
void callback(char* topic, byte * payload, unsigned int length) {
  // handle message arrived
}
void mqttSetup() {
  if (client.connect("ESP8266_872ysrjo21387y"))
    Serial.println("Connected to MQTT broker");
  else {
    Serial.println( "MQTT connect failed");
    Serial.println("/nWill reset and try again...");
    abort();
  }
}

/************************************************************************************
** FUNÇÕES P/ CONEXÃO
************************************************************************************/
void wifiSetup() {
  WiFiManager WFManager;
  if (!WFManager.autoConnect(SSID, PASSWRORD)) {
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
  }
}
/****************** FUNÇÕES MPU6050 ***********************************/
void mpuSetup() {
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, 0x68, SDA_PIN, SCL_PIN))
  {
    Serial.println("[MPU]: MPU6050 não encontrado!");
    delay(500);
  }
}
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
