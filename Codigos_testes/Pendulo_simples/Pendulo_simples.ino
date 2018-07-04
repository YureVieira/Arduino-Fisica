#define pin 5
bool status;
int count = 0;


void setup() {
  Serial.begin(115200);
  pinMode(pin, INPUT);
}

void loop() {
  long time1, time2, time3;
  if (sensor()) {
    time1 = millis();
    while (sensor());
    while (!sensor()) {
      time2 = millis();
    }
    while (sensor());
    while (!sensor()) {
      time3 = millis();
    }
    count++;
    Serial.print(String(count) + "º ciclo / ");
    Serial.println("Periodo: " + String(time3-time1) + " ms");
  }
}

bool sensor() {
  return !digitalRead(pin);
}
