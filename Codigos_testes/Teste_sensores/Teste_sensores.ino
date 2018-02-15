int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A5;
int sensor4 = A3;
int sensor5 = A4;
void setup() {
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
  pinMode (sensor5, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(sensor1));
  Serial.print(", ");
  Serial.print(digitalRead(sensor2));
  Serial.print(", ");
  Serial.print(digitalRead(sensor3));
  Serial.print(", ");
  Serial.print(digitalRead(sensor4));
  Serial.print(", ");
  Serial.print(digitalRead(sensor5));
  Serial.println();
}
