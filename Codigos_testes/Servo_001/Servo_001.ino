#include <Servo.h>

Servo servo;
bool flag_move_servo=false;
int value=10;
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
    for (; value < 100; value++)
      servo.write(value);
    delay(1);
  }
  else
  for (; value > 10; value--){
    servo.write(value);
    delay(1);
  }
  delay(15);
}

