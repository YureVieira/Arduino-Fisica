int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;
int pinTone = 8;

void setup() {
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
  pinMode (sensor5, INPUT);
  pinMode(pinTone, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(sensor1) && digitalRead(sensor2) && digitalRead(sensor3) && digitalRead(sensor4) && digitalRead(sensor5))
    noTone(pinTone);
  else
    tone(pinTone, 800);
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
