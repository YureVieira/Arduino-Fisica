#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <MPU6050.h>              //Biblioteca para o modulo giroscopio/acelerômetro.
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define SENSOR1 D1       //Primeiro sensor.
#define SENSOR2 D2
#define SENSOR3 D5
#define SENSOR4 D6
#define SENSOR5 D7       //Ultimo sensor.
#define SDA_PIN D4
#define SCL_PIN D3
unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
//float T1, T2 , T3, T4;                         //Tempos entre sensores.
//float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
//float distancia = 210.0;
int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;                                          //Instancia da classe usada na leitura do acelerometro/giroscopio.
SSD1306Wire  display(0x3c, SDA_PIN, SCL_PIN);
WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)
const char* ssid = "ESP12E_AP_MODE";
const char* password = "12345678";
String textFix = "GET /forms/d/e/1FAIpQLSdwNWQtjetH4hj_MkRDOeBXkV6ib8LLgmO8th9dk3t6ePcz_g/formResponse?ifq";
String tboxName = "&entry.911139776=Plano_Inclinado";
String tbox1 = "&entry.1827900739=";
String tbox2 = "&entry.1917433581=";
String tbox3 = "&entry.246745679=";
String tbox4 = "&entry.716066615=";
String tbox5 = "&entry.1880225264=";
/************************************************************************************/
void setup()
{
  Serial.begin(115200);
  WiFiManager WFManager;
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
  pinMode(SENSOR1, INPUT_PULLUP); //Botão.
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(SENSOR3, INPUT_PULLUP);
  pinMode(SENSOR4, INPUT_PULLUP);
  pinMode(SENSOR5, INPUT_PULLUP);

  delay(2000);//Espera um tempo para se conectar no WiFi
}
///////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Iniciando em 2s...");
  delay(2000); // aguarda dois segundos
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) {
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

  // Calculo -------------------------------------------
  //  Serial.print("Tempo decorrido 2 (ms): ");
  //  Serial.println(t1 - t0);
  //  Serial.print("Tempo decorrido 3 (ms): ");
  //  Serial.println(t2 - t0);
  //  Serial.print("Tempo decorrido 4 (ms): ");
  //  Serial.println(t3 - t0);
  //  Serial.print("Tempo decorrido 5 (ms): ");
  //  Serial.println(t4 - t0);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
  {
    Serial.println("Conectado ao servidor docs.google.com");
    String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend += tboxName;//Nome do experimento
    toSend += tbox1;// pergunta 1
    toSend += String(t1 - t0);
    toSend += tbox2;// pergunta 2
    toSend += String(t2 - t0);
    toSend += tbox3;// pergunta 3
    toSend += String(t3 - t0);
    toSend += tbox4;// pergunta 4
    toSend += String(t4 - t0);
    toSend += tbox5 + String(getAngleMPU());
    toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
    //    Serial.println(toSend);
    client.println(toSend);//Enviamos o GET ao servidor-
    client.println("Host: docs.google.com");//-
    client.println();//-
    client.stop();//Encerramos a conexao com o servidor
    oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Dados enviados."); //Mostra no monitor que foi enviado
  }
  else
  {
    oledNewMessage(display.getWidth() / 2, display.getHeight() / 2, "Erro ao se conectar"); //Se nao for possivel conectar no servidor, ira avisar no monitor.
  }
  //  Serial.println("Experimento finalizado....");
  //  Serial.println("Reset o ESP8266...");
  delay(3000);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  oledNewMessage(0, 0, "Tempo 1 (ms): " + String(t1 - t0));
  oledAddMessage(0, 0, "\nTempo 2 (ms): " + String(t2 - t0));
  oledAddMessage(0, 0, "\n\nTempo 3 (ms): " + String(t3 - t0));
  oledAddMessage(0, 0, "\n\n\nTempo 4 (ms): " + String(t4 - t0));
  oledAddMessage(0, 0, "\n\n\n\nExperimento finalizado....");
  long counter = millis();
  //  while (millis() - counter < 60000) {
  //    ESP.wdtFeed();
  //  }
  while (1) {
    ESP.wdtFeed();
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
