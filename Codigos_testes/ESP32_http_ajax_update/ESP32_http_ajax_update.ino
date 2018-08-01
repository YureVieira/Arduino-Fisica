#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
ESP8266WebServer server(80);
#else
#include<M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
WebServer server(80);
#endif
#include <ArduinoOTA.h>

const char* ssid = "LME09";
const char* password = "138027Sn";

long data;
/***********************************************************************************************
***********************************************************************************************/
void setup(void) {
  M5.begin();
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  M5.Lcd.print("Connected to ");
  M5.Lcd.println(ssid);
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    M5.Lcd.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/data.txt", updateValues);
  server.onNotFound(handleNotFound);

  server.begin();
  M5.Lcd.println("HTTP server started");
  //Configuração OTA
  ArduinoOTA.setHostname("M5Stack_main");
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
    int w = M5.Lcd.width() - 30;
    int h = 15;
    int value = (progress / (total / 100));
    //    String msg = "Updating " + String(value) + "%";
    //    M5.Lcd.drawCentreString(msg.c_str(), M5.Lcd.width()/2, M5.Lcd.height()/2-20, 2);
    M5.Lcd.drawCentreString("Updating", M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 20, 2);
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
}
/***********************************************************************************************
***********************************************************************************************/
void loop(void) {
  data = millis();
  server.handleClient();
  ArduinoOTA.handle();
}
/***********************************************************************************************
***********************************************************************************************/
void handleRoot() {
  String  page;
  page = "<h1>Sensor to Node MCU Web Server</h1><h1>Data:</h1> <h1 id=\"data\">""</h1>\r\n";
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
  String text = (String)data;
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

