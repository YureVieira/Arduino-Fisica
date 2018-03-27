#include "HX711.h"
#define DOUT 5
#define PD_SCK 4

HX711 cell;

void setup() {
  Serial.begin(115200);
  // parameter "gain" is ommited; the default value 128 is used by the library
cell.begin(DOUT, PD_SCK, 128);//.., .., Gain
  //  cell.set_offset(4018518);
  cell.set_scale(10000.0);
}

void loop() {
  //  Serial.println(cell.read(), 1);
  //Serial.println(cell.get_value(), 1);
  //cell.power_up();
  Serial.println(cell.get_units());
  delay(1);
}
