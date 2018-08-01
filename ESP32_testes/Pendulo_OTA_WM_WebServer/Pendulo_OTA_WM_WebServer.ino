//#define WFM_ENABLE
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define SSID "ESP32E_AP_MODE"
#define PASSWRORD "12345678"
#define SDA_PIN 21
#define SCL_PIN 22
#define pin 23

WebServer server(80);
SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
// SH1106 display(0x3c, D3, D5);
int count = 0;
int _delay = 200;
bool wifi = false;
String _log;

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
  pinMode(pin, INPUT_PULLUP);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema Pronto!IP: "+ WiFi.localIP().toString());
  oledAddMessage(display.getWidth() / 2, display.getHeight() / 2+8, "IP: "+ WiFi.localIP().toString());
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (wifi) {
    serverSetup();
    OTASetup();
  }
}
/************************************************************************************
** LOOP
************************************************************************************/
void loop() {
  uint32_t time1 = 0, time2 = 0, time3 = 0;
  //  if (sensor()) {
  while (!sensor()) {
    if (wifi)combo();
    yield();
  }
  time1 = millis();
  delay(_delay);
  while (!sensor()) {
    if (wifi)combo();
    yield();
  }
  time2 = millis();
  delay(_delay);
  while (!sensor()) {
    if (wifi)combo();
    yield();
  }
  time3 = millis();
  count++;
  String payload = String(time3 - time1);
  oledNewMessage(0, 0, "T1: " + String(time1));
  oledAddMessage(0, 10, "T2: " + String(time2));
  oledAddMessage(0, 20, "T3: " + String(time3));
  oledAddMessage(0, 30, "Periodo: " + String(time3 - time1));
  oledAddMessage(0, 40, "Ciclo: " + String(count));
  _log += String(count) + "º ciclo\n\r";
  _log += "Periodo: " + payload + " ms\n\r";

  if (wifi) combo();
}
void combo() {
  ArduinoOTA.handle();
  server.handleClient();
}
/************************************************************************************
** FUNÇÕES P/ MQTT
************************************************************************************/
//void callback(char* topic, byte * payload, unsigned int length) {
//  // handle message arrived
//}
//bool mqttSetup() {
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  if (client.connect("ESP8266_872ysrjo21387y")) {
//    oledNewMessage(0, 0, "Connected to MQTT broker");
//    delay(1000);
//    return true;
//  }
//  else {
//    oledNewMessage(0, 0, "MQTT connect failed");
//    //    oledAddMessage(0, 10, "Will reset and try again...");
//    delay(5000);
//    return false;
//    //    abort();
//  }
//}
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
/***********************************************************************************************
***********************************************************************************************/
void OTASetup() {
  ArduinoOTA.setHostname("Pendulo_ESP32");
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
/***********************************************************************************************
***********************************************************************************************/
void serverSetup() {
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/data.txt", updateValues);
  server.onNotFound(handleNotFound);
  server.begin();
}
void handleRoot() {
  String  page;
  page = "<h1>Pendulo Simples Web Server</h1><h1>Data:</h1> <h1 id=\"data\">""</h1>\r\n";
  page += "<script>\r\n";
  page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";
  page += "function loadData(url, callback){\r\n";
  page += "var xhttp = new XMLHttpRequest();\r\n";
  page += "xhttp.onreadystatechange = function(){\r\n";
  page += " if(this.readyState == 4 && this.status == 200){\r\n";
  page += " callback.apply(xhttp);\r\n";
  page += " }\r\n";
  page += "};\r\n";
  page += "xhttp.open(\"GET\", url, true);\r\n";
  page += "xhttp.send();\r\n";
  page += "}\r\n";
  page += "function updateData(){\r\n";
  page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
  page += "}\r\n";
  page += "</script>\r\n";
  server.send(200, "text/html", page);
}
void updateValues() {
  String text = _log;
  server.send(200, "text/html", text);
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
