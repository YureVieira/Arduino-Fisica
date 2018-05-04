#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G,0x68,D4,D3))//sda,scl
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

void loop()
{
  //valores de correção obtido impircamente
  int16_t offset = -91;
  int16_t factor = 1;
  int16_t value;
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  int16_t pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  int16_t roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  value = factor * roll + offset;
  
  Serial.print(" angulo = ");
  Serial.print(value);
  
  Serial.println("°");
  
  delay(10);
}


