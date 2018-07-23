#include <WiFi.h>

//const char* ssid = "LME09";
//const char* password =  "138027Sn";
const char* ssid = "Teste32";
const char* password =  "msgdin123";

WiFiServer wifiServer(5052);

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void loop() {

  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {
      Serial.println(".");
      //      while (client.available()>0) {
      ////        char c = client.read();
      client.write("1234");
      //      }

      delay(1000);
    }

    client.stop();
    Serial.println("Client disconnected");

  }
}
void reconecta() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Conexão WiFi perdida: " + String(millis()));
    }
    Serial.println("Reconectado!");
  }
}
