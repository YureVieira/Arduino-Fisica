#include "HX711.h"

HX711 celula;

void setup() {
  Serial.begin(115200);
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  celula.begin(A1, A0, 128);//.., .., Gain
  celula.set_offset(4293966000);
  celula.set_scale(10000.0);
}

void loop() {
//  Serial.println(celula.read(), 1);
//Serial.println(celula.get_value(), 1);
Serial.println(celula.get_units());
  delay(10);
}
