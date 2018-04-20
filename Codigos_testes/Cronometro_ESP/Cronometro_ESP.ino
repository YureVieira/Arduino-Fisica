#define SENSOR1 D1       //Primeiro sensor.
#define SENSOR2 D2
#define SENSOR3 D3
#define SENSOR4 D4
#define SENSOR5 D5       //Ultimo sensor.

unsigned long t0; // tempo inicial
unsigned long t1; // tempo final no sensor2
unsigned long t2; // tempo final no sensor3
unsigned long t3; // tempo final no sensor4
unsigned long t4; // tempo final no sensor5

void setup()
{
  Serial.begin(115200);
  ESP.wdtDisable();//Desabilita o SW WDT. 
  pinMode(SENSOR1, INPUT); //Botão.
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
}

void loop()
{
  //Serial.print("Iniciando em 2s...");
  delay(2000); // aguarda dois segundos

 //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH){ESP.wdtFeed();}//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR1) == LOW){ESP.wdtFeed();}//Momento que o corpo sai do raio de cobertura do sensor
  t0 = millis(); // captura o tempo corrente em t0

  // SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH){ESP.wdtFeed();}//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR2) == LOW){ESP.wdtFeed();}//Momento que o corpo sai do raio de cobertura do sensor
  t1 = millis(); // captura o tempo corrente em t1

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH){ESP.wdtFeed();}
  while (digitalRead(SENSOR3) == LOW){ESP.wdtFeed();}
  t2 = millis(); // captura o tempo corrente em t2

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH){ESP.wdtFeed();}
  while (digitalRead(SENSOR4) == LOW){ESP.wdtFeed();}
  t3 = millis(); // captura o tempo corrente em t3

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH){ESP.wdtFeed();}
  while (digitalRead(SENSOR5) == LOW){ESP.wdtFeed();}
  t4 = millis(); // captura o tempo corrente em t4


  // Calculo -------------------------------------------
  Serial.print("Tempo decorrido 2 (ms): ");
  Serial.println(t1 - t0);
  Serial.print("Tempo decorrido 3 (ms): ");
  Serial.println(t2 - t0);
  Serial.print("Tempo decorrido 4 (ms): ");
  Serial.println(t3 - t0);
  Serial.print("Tempo decorrido 5 (ms): ");
  Serial.println(t4 - t0);


  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  while (1){ESP.wdtFeed();} // loop infinito. reset o arduino para um novo experimento.
}
