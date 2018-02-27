#include <Servo.h>

Servo servo;
bool flag_move_servo;
void setup() {
  servo.attach(9);
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    Serial.read();
    flag_move_servo = !flag_move_servo;
    Serial.println("Byte recebido");
  }
  if (flag_move_servo) {
    servo.write(50);
    delay(10);
    servo.write(100);
  }
  else {
    servo.write(50);
    delay(10);
    servo.write(10);
  }
  delay(15);
}

