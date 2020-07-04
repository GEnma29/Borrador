#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define Volataje_VF1 2
#define Volataje_VF2 3
#define Volataje_VF3 4
LiquidCrystal_I2C lcd(0x27, 16, 2); //config LCD
int Valor;
const int sensorPin1 = A0;    // seleccionar la entrada para el sensor
int sensorValue;       // variable que almacena el valor raw (0 a 1023)
int Valor_Voltaje_VF1; // Variable para testear el voltaje VF1
int Valor_Voltaje_VF2; // Variable para testear el voltaje VF2
int Valor_Voltaje_VF3; // Variable para testear el voltaje VF3
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
  pinMode(Volataje_VF2,INPUT_PULLUP);
  pinMode(Volataje_VF3,INPUT_PULLUP);
  Serial.begin(9600);

}
 
void loop() 
{
  Valor_Voltaje_VF1 = digitalRead(Volataje_VF1); // realizar lectura
  Valor_Voltaje_VF2 = digitalRead(Volataje_VF2); // realizar lectura
  Valor_Voltaje_VF3 = digitalRead(Volataje_VF3); // realizar lectura

  if ((Valor_Voltaje_VF1)&&(Valor_Voltaje_VF2)&&(Valor_Voltaje_VF3))
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Conflicto fase");
    lcd.setCursor(0,1);
    lcd.print("verde");
    delay(200);
  }else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Status Okay");
    delay(200);
  }
  }
