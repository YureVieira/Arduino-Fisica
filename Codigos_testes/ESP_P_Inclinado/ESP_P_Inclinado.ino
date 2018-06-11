#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
/*************************************************************************************/
#define SENSOR1 D3       //Primeiro sensor.
#define SENSOR2 D4
#define SENSOR3 D5
#define SENSOR4 D6
#define SENSOR5 D7       //Ultimo sensor.

unsigned long t0; // tempo inicial
unsigned long t1; // tempo final no sensor2
unsigned long t2; // tempo final no sensor3
unsigned long t3; // tempo final no sensor4
unsigned long t4; // tempo final no sensor5
/***********************************************************************************/

int sda = D2, scl = D1;
SSD1306Wire  display(0x3c, sda, scl);
// SH1106 display(0x3c, D3, D5);


void setup() {
  WiFiManager WFManager;
  Serial.begin(115200);

  if (!WFManager.autoConnect("ESP12E_AP_MODE", "12345678")) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, display.getHeight() / 2, "Falha ao conectar!");
    display.display();
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
  }

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  // Align text vertical/horizontal center
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth() / 2, 10, "IP: " + WiFi.localIP().toString());
  //  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Falha ao conectar!");
  //display.drawXbm(4,32, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();

  pinMode(SENSOR1, INPUT_PULLUP); //Botão.
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);
  delay(5000);
  //Abre o sistema de arquivos (mount)
  openFS();
  //Cria o arquivo caso o mesmo não exista
  createFile("log.txt");
  writeFile("log.txt", "Booting ESP8266...");
  writeFile("log.txt", "Connecting to " + WiFi.SSID());
}

void loop() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Iniciando em 2s...");
  display.display();
  delay(2000); // aguarda dois segundos

  display.clear();
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Sistema pronto!");
  display.display();
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) {
    yield();//Efetua o feed do SW WDT
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR1) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t0 = millis(); // captura o tempo corrente em t0
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "t0 = " + String(t0));
  display.display();

  // SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR2) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t1 = millis(); // captura o tempo corrente em t1
  display.drawString(0, 10, "\nt1 = " + String(t1));
  display.display();

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR3) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t2 = millis(); // captura o tempo corrente em t2
  display.drawString(0, 20, "t2 = " + String(t2));
  display.display();

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR4) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t3 = millis(); // captura o tempo corrente em t3
  display.drawString(0, 30, "t3 = " + String(t3));
  display.display();

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH) {
    yield();
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR5) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t4 = millis(); // captura o tempo corrente em t4
  display.drawString(0, 40, "t4 = " + String(t4));
  display.display();

  // Calculo -------------------------------------------
  delay(2000);
  display.clear();
  display.drawString(0, 0, "Tempo 1 (ms): " + String(t1 - t0));
  display.drawString(0, 10, "Tempo 2 (ms): " + String(t2 - t0));
  display.drawString(0, 20, "Tempo 3 (ms): " + String(t3 - t0));
  display.drawString(0, 30, "Tempo 4 (ms): " + String(t4 - t0));
  display.drawString(0, 40, "Experimento finalizado....");
  display.display();
  String log = "****RESULTADOS****";
  log += "\nTempo 1 (ms): " + String(t1 - t0);
  log += "\nTempo 2 (ms): " + String(t2 - t0);
  log += "\nTempo 3 (ms): " + String(t3 - t0);
  log += "\nTempo 4 (ms): " + String(t4 - t0);
  log += "\nExperimento finalizado";
  writeFile("log.txt",log);
  long counter = millis();
  while (millis() - counter < 50000) {
    ESP.wdtFeed(); // loop de 5 segundos.
  }
  Serial.println(readFile("log.txt"));
}
/*****************************************************************************/
void createFile(String file) {
  File wFile;

  //Cria o arquivo se ele não existir
  if (SPIFFS.exists(file)) {
    Serial.println("Arquivo ja existe!");
  } else {
    Serial.println("Criando o arquivo...");
    wFile = SPIFFS.open(file, "w+");

    //Verifica a criação do arquivo
    if (!wFile) {
      Serial.println("Erro ao criar arquivo!");
    } else {
      Serial.println("Arquivo criado com sucesso!");
    }
  }
  wFile.close();
}

void deleteFile(String file) {
  //Remove o arquivo
  if (SPIFFS.remove(file)) {
    Serial.println("Erro ao remover arquivo!");
  } else {
    Serial.println("Arquivo removido com sucesso!");
  }
}

void writeFile(String file, String msg) {
  File rFile;
  if (!SPIFFS.exists(file)) {
    Serial.println("Arquivo não existe!");
    return;
  }
  rFile = SPIFFS.open(file, "a+");
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.println(msg);
  }
  rFile.close();
}

String readFile(String file) {
  File rFile;
  String result;
  if (!SPIFFS.exists(file)) {
    Serial.println("Arquivo não existe!");
    return "";
  }
  rFile = SPIFFS.open(file, "r");
  Serial.println("Reading file...");
  while (rFile.available()) {
    //    String line = rFile.readStringUntil('\n');
    //    result += line;
    result += (char)rFile.read();
  }
  rFile.close();
  return result;
}

void closeFS(void) {
  SPIFFS.end();
}

void openFS(void) {
  //Abre o sistema de arquivos
  if (!SPIFFS.begin()) {
    Serial.println("Erro ao abrir o sistema de arquivos");
  } else {
    Serial.println("Sistema de arquivos aberto com sucesso!");
  }
}
