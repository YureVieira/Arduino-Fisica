#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//const char* ssid = "LME09";
//const char* password = "138027Sn";
WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)
String textFix = "GET "; //forms/d/e/1FAIpQLSdwNWQtjetH4hj_MkRDOeBXkV6ib8LLgmO8th9dk3t6ePcz_g/formResponse?ifq;
String tboxName = "&entry.911139776=Plano_Inclinado";
String tbox1 = "&entry.1827900739=";
String tbox2 = "&entry.1917433581=";
String tbox3 = "&entry.246745679=";
String tbox4 = "&entry.716066615=";
String tbox5 = "&entry.1880225264=";

long count;
bool flag_ready = false;
bool flag_wait = false;
String sub_url;
WiFiManager WFManager;
///////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
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
  //ArduinoOTA.setPassword("ota01");

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
///////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  String msg = SerialReadLine();
  //Comandos atualmente suportados [URL], [SEND_WIFI], [URL_CLEAR], [NET_CLEAR].
  if (msg == "[URL]") {//Espere pela sub_url apos esse comando
    Serial.println("[ESP_01]: Esperando dados...");
    flag_wait = true;
  }
  else if (msg == "[WIFI_SEND]") {//Prepara para enviar a requisição GET
    Serial.println("[ESP_01]: Pronto para enviar");
    if(sub_url != "" ){
    flag_wait = false;
    flag_ready = true;
    }
    else
    Serial.println("[ESP_01]: URL vazia! Conexão abortada!");
  }
  else if (msg == "[URL_CLEAR]") {//Apague a sub_url atual.
    Serial.println("[ESP_01]: URL Limpa!");
    sub_url = "";
  }
  else if (msg == "[WIFI_CLEAR]") {//Limpe da memoria as credenciais de rede wifi.
    Serial.println("[ESP_01]: Credenciais excluidas");
    WFManager.resetSettings();
    ESP.reset();
    delay(5000);
  }
  else if(flag_wait && msg != ""){//Colete a sub_url passada via serial.
    Serial.println("[ESP_01]: echo: "+ msg);
    sub_url += msg;
    }
  if (flag_ready) {
    digitalWrite(2, 1);
    if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
    {
      Serial.println("Conectado ao servidor docs.google.com");
      String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
      toSend += sub_url;
      toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
      client.println(toSend);//Enviamos o GET ao servidor-
      client.println("Host: docs.google.com");//-
      client.println();//-
      client.stop();//Encerramos a conexao com o servidor
      Serial.println("[ESP_01]: Data submetted :"+toSend);//Mostra no monitor que foi enviado
      digitalWrite(2, 0);
      flag_ready = false;
      sub_url="";
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
///////////////////////////////////////////////////////////////////////////////////////////
String SerialReadLine() {
  static String str = "";
  while (Serial.available()) {
    char data = Serial.read();
    if (data == '\r') {
      Serial.read();
      String aux = String(str);
      str = "";
      return aux;
    }
    else {
      str += data;
    }
  }
  return "";
}
///////////////////////////////////////////////////////////////////////////////////////////
