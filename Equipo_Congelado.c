#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#define int2 2
LiquidCrystal_I2C lcd(0x27, 16, 2); //config LCD
// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.
int LumiVerdeF1 = 0;
int Count_T_LVF1 = 0;
int NumeroSecuencias = 0;
int EstadoLUMVF1;
int Count1 = 0;
int Count2 = 0;
int Primera_Secuencia = 2;
int Bandera1 = 0;
int Bandera0 = 0;
int Segundos = 0;
unsigned long blinkCopy;     // holds a copy of the blinkCount
const int led = LED_BUILTIN; // the pin with a LED
String ejemplo1 = "Bienvenidos!";
String frase = " la Secuencia";
int Selector;
void ContarSecuencia(void)
{
    Count2 = Count_T_LVF1++;
}

void setup(void)
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(ejemplo1);
  pinMode(int2, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Timer1.initialize(10000000);
  Timer1.attachInterrupt(ContarSecuencia); // blinkLED to run every 0.15 seconds
  Serial.begin(9600);
}

void loop(void)
{
while (1)
{
   EstadoLUMVF1 = digitalRead(int2); // Leemos la entrada de la señal del controlador 
   if ((EstadoLUMVF1 == 0)&&(NumeroSecuencias == 0)) // para el caso que empiece en 0
   {
     blinkCopy = 0;
     Count2 = 0;
     lcd.clear();
     lcd.print("Cheak");
     delay(20);
   }
   if ((EstadoLUMVF1 == 1)&&(NumeroSecuencias == 0))
   {
    interrupts();
    ContarSecuencia();
    lcd.clear(); // limpiamos la LCD
    blinkCopy = int(Count2); // almacenamos el valor
    if (blinkCopy > 12048)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("El Controlador");
      lcd.setCursor(0,1);
      lcd.print("esta congelado");
      delay(200);

    }
     
    lcd.setCursor(0,0);
    lcd.print("Segundo");
    lcd.setCursor(0, 1);
    lcd.println(blinkCopy);
    delay(20);
    noInterrupts();
     
   }
   
