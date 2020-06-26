// Programa : Teste Controle Remoto IR   
  
#include <IRremote.h>  
  #define RAWBUF 400
int RECV_PIN = 2;  
float armazenavalor;  
int pinoledvermelho = 3;  
int pinoledverde = 7;  
  
IRrecv irrecv(RECV_PIN);  
decode_results results;  
  
void setup()  
{  
  pinMode(pinoledvermelho, OUTPUT);   
  pinMode(pinoledverde, OUTPUT);  
  Serial.begin(9600);  
  irrecv.enableIRIn(); // Inicializa o receptor IR  
}  
   unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
void loop()  
{  
  int count = results.rawlen;
  if (irrecv.decode(&results))  
  {  
    digitalWrite(pinoledvermelho, HIGH); 
    delay(1000);
    digitalWrite(pinoledvermelho, LOW); 
    
    Serial.print("Valor lido : ");  
    Serial.println(results.value, HEX);
    Serial.println(results.bits, DEC);
    Serial.println(count, DEC);   
    for (int i = 1; i < count; i++) {
    if (i & 1) {
      //Serial.write(' ');
      Serial.print(results.rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write(',');
      Serial.print((unsigned long) results.rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
    Serial.println();
Serial.println("Received unknown code, saving as raw");
    codeLen = results.rawlen - 1;
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" ");
      } 
      else {
        // Space
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" ");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    Serial.println("");
    if (irrecv.decode(&results)) {
      Serial.println("IR code recorded!");
      //irrecv.resume(); // Receive the next value
      Serial.print("Recorded ");
      Serial.print(results.rawlen);
      Serial.println(" intervals.");
    }
    if (results.decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: "+results.decode_type);
  }
  else if (results.decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  }
  else if (results.decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results.decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results.decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results.decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC: ");
  }
  else if (results.decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
   else if (results.decode_type == SAMSUNG) {
    Serial.print("Decoded SAMSUNG: ");
  }
    armazenavalor = (results.value);  
    if (armazenavalor == 0xFF30CF) //Verifica se a tecla 1 foi acionada  
    {  
      digitalWrite(pinoledvermelho, HIGH);  //Acende o led vermelho  
    }  
    if (armazenavalor == 0xFF18E7) //Verifica se a tecla 2 foi acionada  
    {  
      digitalWrite(pinoledvermelho, LOW); //Apaga o led vermelho  
    }  
    if (armazenavalor == 0xFF10EF) //Verifica se a tecla 4 foi acionada  
    {  
      digitalWrite(pinoledverde, HIGH); //Acende o led verde  
    }  
    if (armazenavalor == 0xFF38C7) //Verifica se a tecla 5 foi acionada  
    {  
      digitalWrite(pinoledverde, LOW); //Apaga o led verde  
    }  
    if (armazenavalor == 0xFF52AD) //Verifica se a tecla 9 foi acionada  
    {  
      digitalWrite(pinoledvermelho, LOW); //Apaga todos os leds  
      digitalWrite(pinoledverde, LOW);  
    }  
  irrecv.resume(); //Le o próximo valor  
  }  
}  
