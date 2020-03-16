#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //config LCD
#define Estado_Suministro_Electrico 2
int Valor_Suministro;
String Estatus_Suministro;
void Falla_Suministro(){

  if(Valor_Suministro == LOW)
  {
    Estatus_Suministro = "Off";
    delay(20);
   
  }
  if (Valor_Suministro == HIGH)
  {
    Estatus_Suministro = "On";
    delay(20);
  }

}
void setup()
{
  attachInterrupt(digitalPinToInterrupt(Estado_Suministro_Electrico), Falla_Suministro, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("bienvenidos");
  delay(100);
}
void loop()
{
  Valor_Suministro = digitalRead(Estado_Suministro_Electrico);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Estatus");
  lcd.setCursor(0,1);
  lcd.print("suministro " + Estatus_Suministro);
  delay(200);
 
 /* Valor_Suministro = digitalRead(Estado_Suministro_Electrico);
  
     if (Valor_Suministro == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Estatus OK");
    delay(200);
  }
  if(Valor_Suministro == LOW){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Falla en la red");
    lcd.setCursor(0,1);
    lcd.print("Electrica");
    delay(200);
  }
  */
  
  
   // put your main code here, to run repeatedly:
}
