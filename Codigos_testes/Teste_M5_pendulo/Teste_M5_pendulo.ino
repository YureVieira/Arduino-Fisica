#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <MPU6050.h>

WiFiManager wifiManager;
MPU6050 mpu;
char* _topic = "/acc";
char* server = "10.0.40.168";
WiFiClient wifiClient;
void callback(char* topic, byte * payload, unsigned int length);
PubSubClient client(server, 1883, callback, wifiClient);

void mpuSetup();
void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("Loading networks ...");
  if (!wifiManager.autoConnect("M5Stack_config", "password")) {
    M5.Lcd.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("M5Stack_main");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    M5.Lcd.print("Start updating " + type);
    delay(1000);
    M5.Lcd.clear();
  })
  .onEnd([]() {
    M5.Lcd.clear();
    M5.Lcd.drawCentreString("Restart", M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 20, 3);
    delay(1000);
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    M5.Lcd.setTextColor(GREEN,BLACK );
    int w = M5.Lcd.width() - 30;
    int h = 15;
    int value = (progress / (total / 100));
    //    String msg = "Updating " + String(value) + "%";
    //    M5.Lcd.drawCentreString(msg.c_str(), M5.Lcd.width()/2, M5.Lcd.height()/2-20, 2);
    M5.Lcd.drawCentreString(String("Updating "+String(value)+"%").c_str(), M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 20, 2);
    M5.Lcd.progressBar(15, M5.Lcd.height() / 2, w, h, value);

  })
  .onError([](ota_error_t error) {
    M5.Lcd.clear();
    M5.Lcd.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) M5.Lcd.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) M5.Lcd.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) M5.Lcd.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) M5.Lcd.println("Receive Failed");
    else if (error == OTA_END_ERROR) M5.Lcd.println("End Failed");
  });

  ArduinoOTA.begin();
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());
//  M5.Lcd.print("Largura: ");
//  M5.Lcd.println(M5.Lcd.width());
//  M5.Lcd.print("Altura: ");
//  M5.Lcd.println(M5.Lcd.height());
  mpuSetup();
  mqttSetup();
  M5.Lcd.println("Ready");
  delay(1000);
  M5.Lcd.clear();
}

void loop() {
  int value = getAngleMPU();
  String payload = String(value);
  //Printa o valor atual do angulo
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.drawCentreString("Inclination", M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 24, 4);
  M5.Lcd.drawCentreString("                ", M5.Lcd.width() / 2, M5.Lcd.height() / 2, 4);
  M5.Lcd.drawCentreString(payload, M5.Lcd.width() / 2, M5.Lcd.height() / 2, 4);
  if (client.connected()) {
    if (client.publish("/acc", payload.c_str())) {
      M5.Lcd.setTextColor(GREEN, BLACK);
      M5.Lcd.drawCentreString("Publish ok", M5.Lcd.width() / 2, 0, 4);
      M5.Lcd.drawCentreString(String(millis()), M5.Lcd.width() / 2, 22, 4);
    }
    else {
      M5.Lcd.setTextColor(RED, BLACK);
      M5.Lcd.drawCentreString("Publish failed", M5.Lcd.width() / 2, 0, 4);
      //      Serial.println("Publish failed");
    }
  }
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.drawCentreString("Disconnected", M5.Lcd.width() / 2, 0, 4);
    client.connect("M5Stack_872ysrjo21387y");
  }
  ArduinoOTA.handle();
  delay(100);
  //
  ArduinoOTA.handle();
  M5.update();
  client.loop();
}

/****************** FUNÇÕES MPU6050 ***********************************/
void mpuSetup() {
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, 0x68))
  {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.println("MPU6050 not found!");
    delay(500);
    ArduinoOTA.handle();
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
  value = factor * pitch + offset;
//  value = factor * roll + offset;
  return value;
}
/************************************************************************************
** FUNÇÕES P/ MQTT
************************************************************************************/
void callback(char* topic, byte * payload, unsigned int length) {
  //  String msg;
  //  for (int i = 0; i < length; i++) {
  //    msg += (char)payload[i];
  //  }
  //  M5.Lcd.setTextColor(GREEN, BLACK);
  //  M5.Lcd.drawCentreString(topic, M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 20, 4);
  //  M5.Lcd.drawCentreString("     ", M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);
  //  M5.Lcd.drawCentreString(msg, M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);
}
void mqttSetup() {
  if (client.connect("M5Stack_872ysrjo21387y")) {
    M5.Lcd.println("Connected to MQTT broker");
    client.subscribe(_topic);
  }
  else {
    M5.Lcd.println( "MQTT connect failed");
    M5.Lcd.println("/nWill reset and try again...");
    abort();
  }
}

