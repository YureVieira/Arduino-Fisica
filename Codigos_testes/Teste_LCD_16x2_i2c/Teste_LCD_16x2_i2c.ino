#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
  lcd.begin (16, 2);
  lcd.setBacklight(HIGH);
}

void loop()
{
  //1º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Trilho Honz.");
  lcd.setCursor(0, 1);
  lcd.print("Aguarde valores!");
  delay(1500);
  //2º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tempo1: ");
  lcd.setCursor(8, 0);
  lcd.print("12345678");
  lcd.setCursor(0, 1);
  lcd.print("Tempo2: ");
  lcd.setCursor(8, 1);
  lcd.print("12345678");
  delay(1500);
  //3º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tempo3: ");
  lcd.setCursor(8, 0);
  lcd.print("12345678");
  lcd.setCursor(0, 1);
  lcd.print("Tempo4: ");
  lcd.setCursor(8, 1);
  lcd.print("12345678");
  delay(1500);
  //4º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("vel1: ");
  lcd.setCursor(6, 0);
  lcd.print("1234567890");
  lcd.setCursor(0, 1);
  lcd.print("vel2: ");
  lcd.setCursor(6, 1);
  lcd.print("1234567890");
  delay(1500);
  //5º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("vel3: ");
  lcd.setCursor(6, 0);
  lcd.print("1234567890");
  lcd.setCursor(0, 1);
  lcd.print("vel4: ");
  lcd.setCursor(6, 1);
  lcd.print("1234567890");
  delay(1500);
  //6º tela
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angulo: ");
  lcd.setCursor(8, 0);
  lcd.print("12345678");
  lcd.setCursor(0, 1);
  lcd.print("AG: ");
  lcd.setCursor(4, 1);
  lcd.print("0123456789AB");
  delay(15000);
}
