#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//const char* ssid = "LME09";
//const char* password = "138027Sn";
WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)
String textFix = "GET /forms/d/e/1FAIpQLSdwNWQtjetH4hj_MkRDOeBXkV6ib8LLgmO8th9dk3t6ePcz_g/formResponse?ifq";
String tboxName = "&entry.911139776=Plano_Inclinado";
String tbox1 = "&entry.1827900739=";
String tbox2 = "&entry.1917433581=";
String tbox3 = "&entry.246745679=";
String tbox4 = "&entry.716066615=";
String tbox5 = "&entry.1880225264=";

long count;
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFiManager WFManager;
  digitalWrite(2, 1);
  if (!WFManager.autoConnect("ESP_CONFIG", "ESP45678"))  {
    Serial.println( "Falha ao conectar. Excedeu o tempo limite para conexao.");
    digitalWrite(2, 0);
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
    delay(5000);
  }

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("ESP01");

  // No authentication by default
  ArduinoOTA.setPassword("ota01");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
  });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  for (int i = 0; i < 20; i++) {
    digitalWrite(2, 1);
    delay(50);
    digitalWrite(2, 0);
    delay(50);
  }
  count = millis();
}

void loop() {
  if (millis() - count >= 30000) {
    digitalWrite(2, 1);
    if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
    {
      Serial.println("Conectado ao servidor docs.google.com");
      String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
      toSend += tboxName;//Nome do experimento
      toSend += tbox1;// pergunta 1
      toSend += "Teste";
      toSend += tbox2;// pergunta 2
      toSend += "Teste";
      toSend += tbox3;// pergunta 3
      toSend += "Teste";
      toSend += tbox4;// pergunta 4
      toSend += "Teste";
      toSend += tbox5;// pergunta 5
      toSend += "Teste";
      toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
      client.println(toSend);//Enviamos o GET ao servidor-
      client.println("Host: docs.google.com");//-
      client.println();//-
      client.stop();//Encerramos a conexao com o servidor
      Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
      digitalWrite(2, 0);
      count = millis();
    }
    else {
      Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
      for (int i = 0; i < 3; i++) {
        digitalWrite(2, 1);
        delay(500);
        digitalWrite(2, 0);
        delay(500);
      }
    }
  }
  ArduinoOTA.handle();
}
