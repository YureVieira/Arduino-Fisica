#include <LCD5110_Basic.h>
#define CLK 7
#define DIN 6
#define DC 5
#define RST 3
#define CE 4
LCD5110 tela(CLK, DIN, DC, RST, CE);


//Obtendo as fontes
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()
{
  tela.InitLCD(); //Inicializando o display
}

void loop()
{
  tela.setFont(SmallFont); //Definindo a fonte
  //Escreve a palavra vida alinhada à esquerda começando na linha 0
  tela.print("Contador:", LEFT, 0);
  tela.print(String(millis()), CENTER, 20);
  delay(10);
  tela.clrScr(); //Apaga o contéudo que estiver na tela
}
