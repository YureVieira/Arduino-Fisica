#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Plano_Inclinado";
const char* password = "plano123";
String main_msg;//<meta http-equiv=\"refresh\" content=\"2\">
const char* prefix = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>Plano Inclinado</title><style type=\"text/css\"> body{ font-family: Helvetica, sans-serif; } table,th,td{text-align: center; width: 20%; border: 1px solid black; } </style></head><body><h1>Plano Inclinado</h1><table>";
const char* sufix = "</table></body></html>";
AsyncWebServer server(80);

void setup() {

  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", HTTP_GET, handleRoot);
//  server.onNotFound(handleNotFound);
  server.begin();

  main_msg = "<tr><td><b>Leitura 1(ms):</b></td><td>" ;
  main_msg += String(10 - 20) + "</td></tr>";
  main_msg += "<tr><td><b>Leitura 2(ms)</b></td><td>";
  main_msg += String(200 - 10) + "</td></tr>";
  main_msg += "<tr><td><b>Leitura 3(ms)</b></td><td>";
  main_msg += String(134 - 34) + "</td></tr>";
  main_msg += "<tr><td><b>Leitura 4(ms)</b></td><td>";
  main_msg += String(54 - 23) + "</td></tr>";
  main_msg += "<tr><td><b>Inclinação ( º): </b></td><td>";
  main_msg += String(437) + "</td></tr>";

}

void loop() {
//  server.handleClient();
}
/**************** FUNÇÕES DO WEBSERVER *****************************/
void handleRoot(AsyncWebServerRequest *request) {
  String page = prefix + main_msg +sufix;
  request->send(200, "text/html", page);
}
//void handleNotFound() {
//  String message = "File Not Found\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET) ? "GET" : "POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i = 0; i < server.args(); i++) {
//    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//}
