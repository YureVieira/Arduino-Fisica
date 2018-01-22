int sensor1 = 3;
int sensor2 = 4;
int sensor3 = 5;
int sensor4 = 6;
int sensor5 = 7;

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
float T1, T2, T3, T4;
int leitura;

void setup() {
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
  pinMode (sensor5, INPUT);
  Serial.begin (9600);
}
// Laco principal do programa
void loop() {
  if (digitalRead(sensor1) == LOW) {
    t1 = micros();
    Serial.println("Sensor 1: ");
    //    Serial.print(t1/1000, 3);
    //    Serial.println(" ms");
  }
  if (digitalRead(sensor2) == LOW) {
    t2 = micros();
    Serial.println("Sensor 2: ");
    //    Serial.print(t2/1000, 3);
    //    Serial.println(" ms");
  }
  if (digitalRead(sensor3) == LOW) {
    t3 = micros();
    Serial.println("Sensor 3: ");
    //    Serial.print(t3/1000, 3);
    //    Serial.println(" ms");
  }
  if (digitalRead(sensor4) == LOW) {
    t4 = micros();
    Serial.println("Sensor 4: ");
    //    Serial.print(t4/1000, 3);
    //    Serial.println(" ms");
  }
  if (digitalRead(sensor5) == LOW) {
    t5 = micros();
    Serial.println("Sensor 5: ");
    //    Serial.print(t5/1000, 3);
    //    Serial.println(" ms");
    leitura = 1;
  }
  //Pos leitura
  if ( leitura == 1 ) {
    T1 = (t2 - t1) / 1000.0;
    T2 = (t3 - t2) / 1000.0;
    T3 = (t4 - t3) / 1000.0;
    T4 = (t5 - t4) / 1000.0;
    leitura = 0;
    Serial.println("Leitura: ");
    Serial.println(T1, 3);
    Serial.println(T2, 3);
    Serial.println(T3, 3);
    Serial.println(T4, 3);
    delay(1000);
  }
}
