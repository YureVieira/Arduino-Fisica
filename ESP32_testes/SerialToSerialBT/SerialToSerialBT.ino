#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  String msg = "Hello World!\r\n";
  SerialBT.write((uint8_t*)msg.c_str(),msg.length());
  delay(1000);
}
