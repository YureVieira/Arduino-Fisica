String dist;
float num=0;
void setup() {
  Serial.begin(9600);
  dist = "";
}

void loop() {
  while(Serial.available())
  {
      char dado = Serial.read();
      dist.concat(dado);
      num = dist.toFloat();
  }
  dist = "";
  Serial.println(num,4);
  delay(1000);
}
