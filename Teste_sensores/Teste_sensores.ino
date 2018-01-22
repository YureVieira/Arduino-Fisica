int sensor1 = 3;
int sensor2 = 4;
int sensor3 = 5;
int sensor4 = 6;
int sensor5 = 7;
void setup() {
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
  pinMode (sensor5, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(sensor1);
  Serial.print(", ");
  Serial.print(sensor2);
  Serial.print(", ");
  Serial.print(sensor3);
  Serial.print(", ");
  Serial.print(sensor4);
  Serial.print(", ");
  Serial.print(sensor5);
  Serial.println();
}
