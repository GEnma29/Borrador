#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define Volataje_VF1 2
LiquidCrystal_I2C lcd(0x27, 16, 2); //config LCD
int Valor;
const int sensorPin1 = A0;    // seleccionar la entrada para el sensor
int sensorValue;       // variable que almacena el valor raw (0 a 1023)
int Valor_Voltaja_VF1; // Variable para testear el voltaje VF1
int ValorMaximo = 510;
int ValorMinimo = 486;
int EstadoCorriente;
void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("bienvenidos");
  delay(200);
  pinMode(Volataje_VF1,INPUT_PULLUP);
  Serial.begin(9600);
}
 
void loop() 
{
 // ValorMaximo = 400;
  //ValorMinimo = 524;
   sensorValue = analogRead(sensorPin1);   // realizar la lectura
   Valor_Voltaja_VF1 = digitalRead(Volataje_VF1); // realizar lectura
 
   //mandar mensaje a puerto serie en función del valor leido
   if (sensorValue >= ValorMaximo)
   {
     ValorMaximo = sensorValue;
   }if (sensorValue< ValorMinimo)
   {
     ValorMinimo = sensorValue;
   }
   
   if (ValorMaximo >724)
   {
    // int Epa2 = int(ValorMaximo);
     EstadoCorriente = 1;
     delay(200);
     /*lcd.clear();
     lcd.print("hay corriente");
     lcd.setCursor(0,1);
     lcd.print(Epa2);
     delay(200);*/
     ValorMinimo = 1000;

   }
   if (sensorValue < ValorMaximo)
   {
     if (sensorValue < 214)
     {
     //int Epa3 = int(sensorValue);
     EstadoCorriente = 1;
     delay(2000);
     /*lcd.clear();
     lcd.print("hay corriente");
     delay(200);
     lcd.setCursor(0,1);
     lcd.print(Epa3);
     delay(200);*/
     ValorMinimo = 1000;
     }
     
     if(sensorValue < ValorMinimo)
   {
     ValorMinimo = sensorValue;
   }
   if ((ValorMinimo < 482)&&(ValorMinimo >214))
   {
     EstadoCorriente = 0;
     //int Epa = int(ValorMinimo);
     delay(200);
    /* lcd.clear();
     lcd.print("No Hay corriente");
     lcd.setCursor(0,1);
     lcd.print(Epa);
     delay(200);*/
     ValorMaximo = 300;
   }
   }
   if((Valor_Voltaja_VF1 == 1)&&(EstadoCorriente == 1))
   {
     lcd.clear();
     lcd.print("Estatus Ok");
     lcd.setCursor(0,1);
     lcd.print("Verde On");
     delay(1000);
     ValorMaximo = 400;
     ValorMinimo = 1000;
   }
    if((Valor_Voltaja_VF1 == 1)&&(EstadoCorriente == 0))
   {
     lcd.clear();
     lcd.print("Falla Luminaria");
     lcd.setCursor(0,1);
     lcd.print("Verde fase 1");
     delay(1000);
   }
   if((Valor_Voltaja_VF1 == 0)&&(EstadoCorriente == 0))
   {
     lcd.clear();
     lcd.print("Falla TRIAC");
     lcd.setCursor(0,1);
     lcd.print("Verde fase 1");
     delay(200);
   }
   
   
}
