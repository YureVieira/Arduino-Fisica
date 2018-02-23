// --- Mapeamento de Hardware ---
#define  ADDO  A1    //Data Out
#define  ADSK  A0   //SCK


// --- Protótipo das Funções Auxiliares ---
unsigned long ReadCount(); //conversão AD do HX711


// --- Variáveis Globais ---
unsigned long convert;


// --- Configurações Iniciais ---
void setup()
{
   pinMode(ADDO, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK, OUTPUT);         //saída para SCK
   
   Serial.begin(115200);

} //end setup


// --- Loop Infinito ---
void loop()
{
  convert = ReadCount();
  Serial.println(convert);
  
  delay(10);



} //end loop


// --- Funções ---
unsigned long ReadCount()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK, LOW);
  
  while(digitalRead(ADDO));
  
  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK, LOW);
     if(digitalRead(ADDO)) Count++;
  
  } //end for
  
  digitalWrite(ADSK, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK, LOW);
  
  return(Count);


} //end ReadCount










