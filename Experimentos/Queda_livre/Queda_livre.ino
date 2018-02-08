//Pinos para sensores(entradas analogicas)
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4

unsigned long t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
float T1, T2, T3, T4;
float v1, v2, v3, v4;
int sensor_atual;
int leitura;
float distancia = -1.0;

void setup() {
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);
  pinMode (S5, INPUT);
  Serial.begin (9600);
  Serial.println("Entre com a distancia em milimetros entre os sensores(1 valor)");
  String distancia_str = "";
  while (1)
  {
    while (Serial.available()) {
      char dado = Serial.read();
      distancia_str.concat(dado);
    }
    distancia = distancia_str.toFloat();
    if (distancia > 0) {
      Serial.print("Distancia entre sensores configurada para: ");
      Serial.print(distancia);
      Serial.println(" mm");
      distancia=distancia/1000;
      break;
    }
    else {
      Serial.println("Entre com um valor valido!");
      distancia_str = "";
    }
    delay(1000);
  }
  sensor_atual = 5;
}
// Laco principal do programa
void loop() {
  if (distancia > 0.0)
  {
    if (digitalRead(S1) == LOW && sensor_atual == 5) {
      t1 = millis();
      sensor_atual = 1;
      Serial.println("Sensor1");
    }
    if (digitalRead(S2) == LOW && sensor_atual == 1) {
      t2 = millis();
      sensor_atual = 2;
      Serial.println("Sensor2");
    }
    if (digitalRead(S3) == LOW && sensor_atual == 2) {
      t3 = millis();
      sensor_atual = 3;
      Serial.println("Sensor3");
    }
    if (digitalRead(S4) == LOW && sensor_atual == 3) {
      t4 = millis();
      sensor_atual = 4;
      Serial.println("Sensor4");
    }
    if (digitalRead(S5) == LOW && sensor_atual == 4) {
      t5 = millis();
      sensor_atual = 5;
      Serial.println("Sensor5");
      leitura = 1;
    }
    //Pos leitura
    if ( leitura == 1 ) {      
      T1 = (t2 - t1) / 1000.0;
      T2 = (t3 - t1) / 1000.0;
      T3 = (t4 - t1) / 1000.0;
      T4 = (t5 - t1) / 1000.0;

      //Calculo da Velocidade
      v1 = distancia / T1;
      v2 = 2*distancia / T2;
      v3 = 3*distancia / T3;
      v4 = 4*distancia / T4;
      
      leitura = 0;
      Serial.println("############ TEMPOS ################ ");
      Serial.print(T1, 6);
      Serial.println(" s");
      Serial.print(T2, 6);
      Serial.println(" s");
      Serial.print(T3, 6);
      Serial.println(" s");
      Serial.print(T4, 6);
      Serial.println("########### VELOCIDADES ############ ");
      Serial.print(v1, 6);
      Serial.println(" s");
      Serial.print(v2, 6);
      Serial.println(" s");
      Serial.print(v3, 6);
      Serial.println(" s");
      Serial.print(v4, 6);
      Serial.println(" m/s");
      Serial.println("#################################### ");
      delay(1000);
    }
  }
}
