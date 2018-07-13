#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "BluetoothSerial.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

const char* ssid = "LME09";
const char* password = "138027Sn";
SSD1306Wire  display(0x3c, 22, 21);//SDA,SCL
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Bluetooth"); //Bluetooth device name
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

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
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 10, "OTA Update");
    display.display();
  })
  .onEnd([]() {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Restart");
    display.display();
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
    display.display();
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Ready for OTA:\n" + WiFi.localIP().toString());
  display.display();
}
unsigned int count=0;
String prefix = "Tempo";
void loop() {
  ArduinoOTA.handle();
//  String msg = "Hello World!!\r\n";
//  SerialBT.write((uint8_t*)msg.c_str(), msg.length());
//  Serial.println(msg.length());

  String msg = prefix + "1|"+count;
  SerialBT.write((uint8_t*)msg.c_str(), msg.length());
  delay(1000);
  msg = prefix + "2|"+count;
  SerialBT.write((uint8_t*)msg.c_str(), msg.length());
  delay(1000);
  msg = prefix + "3|"+count;
  SerialBT.write((uint8_t*)msg.c_str(), msg.length());
  delay(1000);
  msg = prefix + "4|"+count;
  SerialBT.write((uint8_t*)msg.c_str(), msg.length());
  delay(1000);
  count++;
}
