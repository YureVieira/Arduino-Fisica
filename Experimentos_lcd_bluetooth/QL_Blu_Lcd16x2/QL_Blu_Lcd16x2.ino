/*--------------------------------------------------------------------------
  UNIVERSIDADE FEDERAL DO CEARÁ - UFC
  LABORATÓRIO DE MÍDIAS EDUCACIONAIS - LME
  PROJETO: Física com Arduino
  ANALISTA: Antonio de Lisboa Coutinho Junior
  DESENVOLVEDOR: Yure Vieira Sampaio Albuquerque
  DATA: 10/04/2018
  VERSÃO: 4.0
  REPOSITÓRIO: GOOGLE DRIVE\UAB - Arduino\Código Arduino

  DESCRIÇÃO:
  Código para coleta de dados do instrumento QUEDA LIVRE.
  Possui cinco sensores de obstáculo para detectar um objeto
  que se desloque em queda livre.
  Conectorização em placa Arduino Uno R3.
  Serão tomados 4 tempos.
  Calculada a velocidade do objeto.
  Transmissão via módulo Serial/Bluetooth.
  Dados são mostrados em um modulo lcd 16x2 i2c.

  --------------------------------------------------------------------------*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //Biblioteca para o modulo LCD-i2c.
#include <SoftwareSerial.h>       //Biblioteca usada com bluetooth.

#define SENSOR1 8       //Primeiro sensor.
#define SENSOR2 9
#define SENSOR3 10
#define SENSOR4 11
#define SENSOR5 12      //Ultimo sensor.
#define BOTAO 7         //Botão para funções.
#define S_TX 2          //Tx para bluetooth.
#define S_RX 3          //Rx para bluetooth.

unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0; //Registros de tempo para cada sensor apartir de millis().
float T1, T2, T3, T4;                                 //Tempos entre sensores.
float v1, v2, v3, v4;                                 //Velocidades médias durante a passagem por cada cada sensor.
float distancia = 210.0;                              //Distancia entre sensores.

// Inicializa o display no endereco 0x3f.
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//Inicializa a interface serial para bluetooth
SoftwareSerial bluetooth(S_RX, S_TX);

//Apaga todo display e escreve uma mensagem começando pela posição (i,j).
void LCD_NovaMensagem(String str, int i = 0, int j = 0) {
  lcd.clear();
  lcd.setCursor(i, j);
  lcd.print(str);
}
//Escreve uma mensagem começando pela posição (i,j).
void LCD_Mensagem(String str, int i = 0, int j = 0) {
  lcd.setCursor(i, j);
  lcd.print(str);
}

void setup()
{
  pinMode(SENSOR1, INPUT); //Botão.
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(BOTAO, INPUT_PULLUP);//Pullup interno despensa resistor externo.
  Serial.begin (115200);//Comunicação serial configurada.
  bluetooth.begin(9600);//Porta para bluetooth.
  lcd.begin (16, 2);    //Especifica tamalho do display.
  lcd.setBacklight(HIGH);//Acende backlight.
}

void loop()
{
  delay(2000); // aguarda dois segundos.
  //Ultima mensagem enviada a serial antes dos valores colhidos do experimento.
  Serial.println("[START]");
  //Mensagem padrão no lcd.
  LCD_NovaMensagem("Queda Livre");
  LCD_Mensagem("Aguardando valores!", 0, 1);
  //SENSOR1 -------------------------------------------
  while (digitalRead(SENSOR1) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR1) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t0 = millis(); // captura o tempo corrente em t0

  // SENSOR2 -------------------------------------------
  while (digitalRead(SENSOR2) == HIGH);//Momento que o corpo entra no raio de cobertura do sensor
  while (digitalRead(SENSOR2) == LOW);//Momento que o corpo sai do raio de cobertura do sensor
  t1 = millis(); // captura o tempo corrente em t1

  //SENSOR3---------------------------------------------
  while (digitalRead(SENSOR3) == HIGH);
  while (digitalRead(SENSOR3) == LOW);
  t2 = millis(); // captura o tempo corrente em t2

  //SENSOR4---------------------------------------------
  while (digitalRead(SENSOR4) == HIGH);
  while (digitalRead(SENSOR4) == LOW);
  t3 = millis(); // captura o tempo corrente em t3

  //SENSOR5---------------------------------------------
  while (digitalRead(SENSOR5) == HIGH);
  while (digitalRead(SENSOR5) == LOW);
  t4 = millis(); // captura o tempo corrente em t4

  LCD_NovaMensagem("Dados coletados!");
  // Calculo -------------------------------------------
  T1 = (t1 - t0) ;
  T2 = (t2 - t0) ;
  T3 = (t3 - t0) ;
  T4 = (t4 - t0) ;

  //Calculo da Velocidade.
  v1 = distancia / T1;
  v2 = 2 * distancia / T2;
  v3 = 3 * distancia / T3;
  v4 = 4 * distancia / T4;

  //Dados para graficos em python
  Serial.println(T1);
  Serial.println(T2);
  Serial.println(T3);
  Serial.println(T4);

  //Dados enviados por bluetooth
  bluetooth.print("Tempo1|");
  bluetooth.print(T1);
  delay(1000);
  bluetooth.print("Tempo2|");
  bluetooth.print(T2);
  delay(1000);
  bluetooth.print("Tempo3|");
  bluetooth.print(T3);
  delay(1000);
  bluetooth.print("Tempo4|");
  bluetooth.print(T4);
  delay(1000);

  Serial.println("Experimento finalizado....");
  Serial.println("Reset o Arduino...");
  while (1) { // loop infinito. reset o arduino para um novo experimento.
    //Dados são mostrados no display, necessário pressionar botão para mostrar todos os dados
    LCD_NovaMensagem("Tempo1: ");
    LCD_Mensagem(String(T1), 8, 0);
    LCD_Mensagem("Tempo2: ", 0, 1);
    LCD_Mensagem(String(T2) , 8, 1);
    delay(200);
    while (digitalRead(BOTAO) == HIGH);
    while (digitalRead(BOTAO) == LOW);
    LCD_NovaMensagem("Tempo3: ");
    LCD_Mensagem(String(T3), 8, 0);
    LCD_Mensagem("Tempo4: ", 0, 1);
    LCD_Mensagem(String(T4) , 8, 1);
    delay(200);
    while (digitalRead(BOTAO) == HIGH);
    while (digitalRead(BOTAO) == LOW);
    LCD_NovaMensagem("Vel1: ");
    LCD_Mensagem(String(T1), 6, 0);
    LCD_Mensagem("Vel2: ", 0, 1);
    LCD_Mensagem(String(T2) , 6, 1);
    delay(200);
    while (digitalRead(BOTAO) == HIGH);
    while (digitalRead(BOTAO) == LOW);
    LCD_NovaMensagem("Vel3: ");
    LCD_Mensagem(String(T3), 6, 0);
    LCD_Mensagem("Vel4: ", 0, 1);
    LCD_Mensagem(String(T4), 6, 1);
    delay(200);
    //    while (digitalRead(BOTAO) == HIGH);
    //    LCD_NovaMensagem("Angulo: ");
    //    LCD_Mensagem("????", 8, 0);
    //    LCD_Mensagem("AG: ", 0, 1);
    //    LCD_Mensagem("????", 4, 1);
    //    delay(200);
    while (digitalRead(BOTAO) == HIGH);
    while (digitalRead(BOTAO) == LOW);
  }
}
