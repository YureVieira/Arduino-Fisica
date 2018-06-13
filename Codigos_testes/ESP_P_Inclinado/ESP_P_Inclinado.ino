#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <MPU6050.h>
/*************************************************************************************/
#define SENSOR1 D3       //Primeiro sensor.
#define SENSOR2 D4
#define SENSOR3 D5
#define SENSOR4 D6
#define SENSOR5 D7       //Ultimo sensor.
#define SDA_PIN D2
#define SCL_PIN D1

unsigned long t0; // tempo inicial
unsigned long t1; // tempo final no sensor2
unsigned long t2; // tempo final no sensor3
unsigned long t3; // tempo final no sensor4
unsigned long t4; // tempo final no sensor5
/***********************************************************************************/
const char* ssid = "ESP12E_AP_MODE";
const char* password = "12345678";
const char* INDEX_HTML = "<!DOCTYPE html> <html> <head> <!--<meta http-equiv=\"refresh\" content=\"2\">--> <title>Plano Inclinado</title> <style type=\"text/css\"> body{ font-family: Helvetica, sans-serif; font-weight: bold; } .button { border: none; border-radius: 8px; padding: 11px 22px; text-align: center; font-size: 16px; margin: 4px 2px; /*transition-duration: 0.4s;*/ cursor: pointer; } input[type=submit] { background-color: #f6863c; background-image: -webkit-gradient(linear, left top, left bottom, from(#f8a56d), to(#f6863c)); color: white; } input[type=submit]:hover { /*border:1px solid #ff9913;*/ background-color: #ddd; background-image: -webkit-gradient(linear, left top, left bottom, from(#fff), to(#bbb)); color: black; } </style> </head> <body> <h1>Plano Inclinado</h1> <div class=\"topMenu-div\"> <a href=\"\">Dimensões</a> <a href=\"\">Rede</a> <a href=\"\">Log</a> <a href=\"\">Sobre</a> </div> <div class=\"form-div\"> <form class=\"form-style\" action=\"/\" method=\"get\"> <p><label>1º Tempo:</label> <input type=\"text\" name=\"input1\"></p> <p><label>2º Tempo:</label> <input type=\"text\" name=\"input2\"></p> <p><label>3º Tempo:</label> <input type=\"text\" name=\"input3\"></p> <p><label>4º Tempo:</label> <input type=\"text\" name=\"input4\"></p> <hr> <div id=\"time-input\"> <p> <label>Unidade de tempo:</label> <select name=\"time\"> <option value=\"1\">Segundos(s)</option> <option value=\"1000\">Milisegundos(ms)</option> <option value=\"1000000\">Microsegundos(us)</option> </select> </p> </div> <div id=\"space-input\"> <p> <label>Unidade de espaço:</label> <select name=\"space\"> <option value=\"1\">Metros(m)</option> <option value=\"1000\">Milimetros(mm)</option> </select> </p> </div> <div class=\"submit-div\"> <input class=\"button\" type=\"submit\" value=\"Ok\"> </div> </form> </div> </body> </html>";

ESP8266WebServer server(80);//webserver
SSD1306Wire  display(0x3c, SDA, SCL);
int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  WiFiManager WFManager;
  //  WFManager.resetSettings();
  //Inicializa display oled
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(0, 0, "[MPU]: Inicializando MPU6050");

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, 0x68, SDA_PIN, SCL_PIN))
  {
    oledNewMessage(0, 0, "[MPU]: MPU6050 não encontrado!");
    delay(500);
  }
  oledNewMessage(0, 0, "*WIFI: Configure o WiFi!");
  oledAddMessage(0, 0, "\n*WIFI: Acesse a rede: " + String(ssid) + " Senha:" + String(password) + "\nIP: 192.168.4.1");
  if (!WFManager.autoConnect(ssid, password)) {
    //    oledNewMessage(0, display.getHeight() / 2, "[WIFI]: Falha ao conectar !");
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
  }
  // Align text vertical/horizontal center
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "IP: " + WiFi.localIP().toString());
  //display.drawXbm(4,32, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);

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
  writeFile("log.txt", "Arrancando ESP8266...");
  writeFile("log.txt", "Conectado a rede " + WiFi.SSID());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Iniciando em 2s...");
  delay(2000); // aguarda dois segundos
  //  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Sistema pronto!");

  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) {
    server.handleClient();
    oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Inclinação: " + String(getAngleMPU()) + "°");
    yield();//Efetua o feed do SW WDT
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR1) == LOW) {
    yield();
    ESP.wdtFeed();
  }
  t0 = millis(); // captura o tempo corrente em t0
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  oledNewMessage(0, 0, "t0 = " + String(t0));

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
  oledAddMessage(0, 11, "t1 = " + String(t1));

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
  oledAddMessage(0, 22, "t2 = " + String(t2));

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
  oledAddMessage(0, 33, "t3 = " + String(t3));

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
  oledAddMessage(0, 44, "t4 = " + String(t4));
  server.handleClient();
  // Calculo -------------------------------------------
  delay(2000);

  oledNewMessage(0, 0, "Tempo 1 (ms): " + String(t1 - t0));
  oledAddMessage(0, 0, "\nTempo 2 (ms): " + String(t2 - t0));
  oledAddMessage(0, 0, "\n\nTempo 3 (ms): " + String(t3 - t0));
  oledAddMessage(0, 0, "\n\n\nTempo 4 (ms): " + String(t4 - t0));
  oledAddMessage(0, 0, "\n\n\n\nExperimento finalizado....");

  String log = "****RESULTADOS****";
  log += "\nTempo 1 (ms): " + String(t1 - t0);
  log += "\nTempo 2 (ms): " + String(t2 - t0);
  log += "\nTempo 3 (ms): " + String(t3 - t0);
  log += "\nTempo 4 (ms): " + String(t4 - t0);
  log += "\nExperimento finalizado";
  //Escreva no display
  //  oledNewMessage(0,0,log);
  //Escreva no arquivo
  writeFile("log.txt", log);
  //Escreva no HTML
//  String indexMod = 
  //Laço de 50s
  long counter = millis();
  while (millis() - counter < 50000) {
    server.handleClient();
    ESP.wdtFeed(); // loop de 5 segundos.
  }
  Serial.println(readFile("log.txt"));
}
/****************** FUNÇÕES P/ LEITURA/ESCRITA DE ARQUIVOS**************/
void formatFS() {
  SPIFFS.format();
}
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
/****************** FUNÇÕES P/ OLED ***********************************/
void oledNewMessage(int16_t x, int16_t y, String msg) {
  display.clear();
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
void oledAddMessage(int16_t x, int16_t y, String msg) {
  display.drawStringMaxWidth(x, y, display.getWidth(), msg);
  display.display();
}
/****************** FUNÇÕES MPU6050 ***********************************/
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
/**************** FUNÇÕES DO WEBSERVER *****************************/
void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
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
/************************* DIVERSOS **************************/

