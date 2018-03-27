#include "HX711.h"
#define DOUT 5
#define PD_SCK 4

HX711 cell;
double tare=0;

void setup() {
  Serial.begin(115200);
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  cell.begin(DOUT, PD_SCK, 64);//.., .., Gain
  //  cell.set_offset(4018518);
  cell.set_scale(10000.0);
  //  tempo para calibração
  Serial.println("Estabilizando");
  long time1 = millis();
  while (millis() - time1 < 10000);
  Serial.println("Calibrando");
  time1 = millis();
  int count;
  while (millis() - time1 < 10000) {
    tare = tare + cell.get_units();
    count++;
    delay(10);
    Serial.print(".");
  }
  Serial.println();
  tare=tare/count;
  Serial.println("Tara =" + String(tare));
}

void loop() {
  //  Serial.println(cell.read(), 1);
  //Serial.println(cell.get_value(), 1);
  //cell.power_up();
  double value = cell.get_units();
  Serial.print("valor lido: ");
  Serial.print(value);
  Serial.print("     valor corrigido: ");
  Serial.println(value - tare);
  //delay(10);
}
