#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define SENSOR1 D1       //Primeiro sensor.
#define SENSOR2 D2
#define SENSOR3 D5
#define SENSOR4 D6
#define SENSOR5 D7       //Ultimo sensor.
#define SDA_PIN D4
#define SCL_PIN D3

unsigned long t0; // tempo inicial
unsigned long t1; // tempo final no sensor2
unsigned long t2; // tempo final no sensor3
unsigned long t3; // tempo final no sensor4
unsigned long t4; // tempo final no sensor5

int angulo;                                           //Angulo de inclinação.
MPU6050 mpu;

WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora
String textFix = "GET /forms/d/e/1FAIpQLSdwNWQtjetH4hj_MkRDOeBXkV6ib8LLgmO8th9dk3t6ePcz_g/formResponse?ifq";
String tboxName = "&entry.911139776=Plano_Inclinado";
String tbox1 = "&entry.1827900739=";
String tbox2 = "&entry.1917433581=";
String tbox3 = "&entry.246745679=";
String tbox4 = "&entry.716066615=";
String tbox5 = "&entry.1880225264=";

/*****************************************************/
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

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialize MPU6050");
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, 0x68, SDA_PIN, SCL_PIN))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  WiFiManager WFManager;
  //WFManager.resetSettings();
  if (!WFManager.autoConnect("ESP12E_AP_MODE", "12345678")) {
    Serial.println( "Falha ao conectar. Excedeu o tempo limite para conexao.");
    delay(3000);
    //Reinicia o ESP e tenta novamente ou entra em sono profundo (DeepSleep)
    ESP.reset();
    delay(5000);
  }
  //ESP.wdtDisable();//Desabilita o SW WDT.
  Serial.println("Conectado!!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP().toString());
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
  Serial.println();
  Serial.println("Iniciando em 2s...");
  delay(2000);
  Serial.print("angulo = ");
  Serial.print(getAngleMPU());
  Serial.println("°");
  Serial.println("Sistema Pronto");
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR1) == LOW) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  t0 = millis(); // captura o tempo corrente em t0
  Serial.println("t0");

  // SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR2) == LOW) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  t1 = millis(); // captura o tempo corrente em t1
  Serial.println("t1");

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR3) == LOW) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  t2 = millis(); // captura o tempo corrente em t2
  Serial.println("t2");

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR4) == LOW) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  t3 = millis(); // captura o tempo corrente em t3
  Serial.println("t3");

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  while (digitalRead(SENSOR5) == LOW) {
    yield();//Efetua o feed do SW WDT.
    ESP.wdtFeed();
  }
  t4 = millis(); // captura o tempo corrente em t4
  Serial.println("t4");

  // Calculo -------------------------------------------
  Serial.print("Tempo decorrido 2 (ms): ");
  Serial.println(t1 - t0);
  Serial.print("Tempo decorrido 3 (ms): ");
  Serial.println(t2 - t0);
  Serial.print("Tempo decorrido 4 (ms): ");
  Serial.println(t3 - t0);
  Serial.print("Tempo decorrido 5 (ms): ");
  Serial.println(t4 - t0);
  //Leitura do angulo
  angulo = getAngleMPU();
  Serial.print("Angulo: ");
  Serial.println(angulo);

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
    toSend += tbox5;
    toSend += String(angulo);
    toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
    Serial.println(toSend);
    client.println(toSend);//Enviamos o GET ao servidor-
    client.println("Host: docs.google.com");//-
    client.println();//-
    client.stop();//Encerramos a conexao com o servidor
    Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
  }
  else
  {
    Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
  }

  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  long counter = millis();
  while (millis() - counter < 5000) {
    ESP.wdtFeed(); // loop de 5 segundos.
  }
}
