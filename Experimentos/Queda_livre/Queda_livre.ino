//Pinos para sensores
#define S1 5
#define S2 6
#define S3 7
#define S4 8
#define S5 9

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
float T1, T2, T3, T4;
int leitura;

void setup() {
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  Serial.begin (9600);
}
// Laco principal do programa
void loop() {
  if (digitalRead(S1) == LOW) {
    t1 = micros();
    Serial.println("Sensor1");
  }
  if (digitalRead(S2) == LOW) {
    t2 = micros();
    Serial.println("Sensor2");
  }
  if (digitalRead(S3) == LOW) {
    t3 = micros();
    Serial.println("Sensor3");
  }
  if (digitalRead(S4) == LOW) {
    t4 = micros();
    Serial.println("Sensor4");
  }
  if (digitalRead(S5) == LOW) {
    t5 = micros();
    Serial.println("Sensor5");
    leitura = 1;
  }
  //Pos leitura
  if ( leitura == 1 ) {
    T1 = (t2 - t1) / 1000.0;
    T2 = (t3 - t2) / 1000.0;
    T3 = (t4 - t3) / 1000.0;
    T4 = (t5 - t4) / 1000.0;
    leitura = 0;
    Serial.println("############ LEITURA ############### ");
    Serial.println(T1, 3);
    Serial.println(T2, 3);
    Serial.println(T3, 3);
    Serial.println(T4, 3);
    Serial.println("#################################### ");
    delay(1000);
  }
}
