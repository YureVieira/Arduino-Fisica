int sensor1 = 4;
int sensor2 = 5;
int sensor3 = 6;
int sensor4 = 7;
int sensor5 = 8;
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
