#include "U8glib.h"
#include <stdio.h>
// A linha abaixo define as ligacoes e deve ser
// ajustada conforme o display utilizado.
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 , 7); //Enable, RW, RS, RESET

void u8g_prepare()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void u8g_Tela1(int num)
{
  char timeon[11];
  sprintf(timeon, "%u", num);
  u8g.drawStr( 0, 0, "loop: ");
  u8g.drawStr( 0, 10, timeon);
  //  u8g.drawStr(0, 0, "Arduino inicializado !");
  //  u8g.drawStr(0, 10, "Sistema pronto !");
}

void draw(int num) //Rotina Desenho
{
  u8g_prepare();
  u8g_Tela1(num);
}

void setup()
{
  // flip screen, if required
//  u8g.setRot180();

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);   // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(1);     // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);     // pixel on

  //u8g.setContrast(0x30);
}

void loop()
{
  int i;
  // picture loop
  for (i = 0; i < 180; i++)
  {
    u8g.firstPage();
    do
    {
      draw(i);
    }
    while ( u8g.nextPage() );
    delay(500);  //Pausa de 3 segundos e reinicia o processo
  }
}
