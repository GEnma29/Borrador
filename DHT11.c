/*
 * File:   DHT11.c
 * Author: guest-fa0bca58-64ec-4822-b00c-7bba82399568@transim.com
 *
 * Created on 5/27/2020 5:27:24 PM UTC
 * "Created in MPLAB Xpress"
 */


#include <xc.h>

#define Data_Pin   PORTAbits.RA2                      // Pin mapped to PORTA.RA2
#define Data_Pin_Direction TRISAbits.TRISA2            // Pin direction mapped to TRISD.0
 
char message1[] = "Temp = 00.0 C";
char message2[] = "RH   = 00.0 %";
short Time_out;
unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum ;
void start_signal(){
  Data_Pin_Direction = 0;              // Configure connection pin as output
  Data_Pin = 0;                        // Connection pin output low
  delay_ms(25);
  Data_Pin = 1;                        // Connection pin output high
  delay_us(30);
  Data_Pin_Direction = 1;              // Configure connection pin as input
}
short check_response(){
  delay_us(40);
  if(!Data_Pin){                     // Read and test if connection pin is low
    delay_us(80);
    if(Data_Pin){                    // Read and test if connection pin is high
      delay_us(50);
      return 1;}
    }
}
unsigned int8 Read_Data(){
  unsigned int8 i, k, _data = 0;     // k is used to count 1 bit reading duration
  if(Time_out)
    break;
  for(i = 0; i < 8; i++){
    k = 0;
    while(!Data_Pin){                          // Wait until pin goes high
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_us(1);}
    delay_us(30);
    if(!Data_Pin)
      bit_clear(_data, (7 - i));               // Clear bit (7 - i)
    else{
      bit_set(_data, (7 - i));                 // Set bit (7 - i)
      while(Data_Pin){                         // Wait until pin goes low
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_s(1);}
    }
  }
  return _data;
}
void main(){
  PIC_Configuracion_Inicial();
  LCD_Configuracion_Inicial(); 
  while(TRUE){
    delay_ms(1000);
    Time_out = 0;
    Start_signal();
    if(check_response()){                    // If there is response from sensor
      RH_byte1 = Read_Data();                 // read RH byte1
      RH_byte2 = Read_Data();                 // read RH byte2
      T_byte1 = Read_Data();                  // read T byte1
      T_byte2 = Read_Data();                  // read T byte2
      Checksum = Read_Data();                 // read checksum
      if(Time_out){                           // If reading takes long time
        LCD_Comando(0x02);                      // LCD clear
        LCD_Cursor(5, 1);                     // Go to column 5 row 1
        LCD_EscribirStr("Time out!");
      }
      else{
       if(CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)){
        message1[7]  = T_Byte1/10  + 48;
        message1[8]  = T_Byte1%10  + 48;
        message1[10] = T_Byte2/10  + 48;
        message2[7]  = RH_Byte1/10 + 48;
        message2[8]  = RH_Byte1%10 + 48;
        message2[10] = RH_Byte2/10 + 48;
        message1[11] = 223;                   // Degree symbol
        LCD_Comando(0x02);                       // LCD clear
        LCD_Cursor(1, 1);                     // Go to column 1 row 1
        LCD_EscribirStr(message1);           // Display message1
        LCD_Cursor(1, 2);                     // Go to column 1 row 2
        LCD_EscribirStr(message2);          // Display message2
       }
        else {
          LCD_Comando(0x02);                     // LCD clear
          LCD_Cursor(1, 1);                  // Go to column 1 row 1
          LCD_EscribirStr("Checksum Error!");
        }
      }
    }
    else {
      LCD_Comando(0x02);             // LCD clear
      LCD_Cursor(3, 1);             // Go to column 3 row 1
      LCD_EscribirStr("No response");
      LCD_Cursor(1, 2);           // Go to column 1 row 2
      LCD_EscribirStr("from the sensor");
    }
  }
}
void __interrupt(high_priority) Interrupcion(void) {
    if (RBIF) //Si hay cambio de estado en PORTB
    {
        char a = 0, b = 1, c = 1, i = 1, k = 1;
        while (k == 1) {
            PORTBbits.RB3 = a;
            PORTBbits.RB2 = b;
            PORTBbits.RB1 = c;


            if ((PORTBbits.RB7 == 0)&&(PORTBbits.RB3 == 0)) //Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('*');
                n = '*';
                k = 0;
                __delay_ms(100);

            }
            if ((PORTBbits.RB7 == 0)&&(PORTBbits.RB2 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('0');
                n = '0';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB7 == 0)&&(PORTBbits.RB1 == 0))//Código de atención de la interrupción
            {
                __delay_ms(2);
                //LCD_Escribir('#');
                n = '#';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB6 == 0)&&(PORTBbits.RB3 == 0)) //Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('7');
                n = '7';
                k = 0;
                __delay_ms(100);

            }
            if ((PORTBbits.RB6 == 0)&&(PORTBbits.RB2 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('8');
                n = '8';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB6 == 0)&&(PORTBbits.RB1 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('9');
                n = '9';
                k = 0;
                __delay_ms(100);
            }
            //
            if ((PORTBbits.RB5 == 0)&&(PORTBbits.RB3 == 0)) //Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('4');
                n = '4';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB5 == 0)&&(PORTBbits.RB2 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('5');
                n = '5';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB5 == 0)&&(PORTBbits.RB1 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('6');
                n = '6';
                k = 0;
                __delay_ms(100);
            }
            //
            if ((PORTBbits.RB4 == 0)&&(PORTBbits.RB3 == 0)) //Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('1');
                n = '1';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB4 == 0)&&(PORTBbits.RB2 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('2');
                n = '2';
                k = 0;
                __delay_ms(100);
            }
            if ((PORTBbits.RB4 == 0)&&(PORTBbits.RB1 == 0))//Código de atención de la interrupción
            {
                //LCD_Comando(0x01); //Display clear
                __delay_ms(2);
                //LCD_Escribir('3');
                n = '3';
                k = 0;
                __delay_ms(100);
            }

            if (i == 1) {
                a = 1;
                b = 0;
                c = 1;
            }
            if (i == 2) {
                a = 1;
                b = 1;
                c = 0;
                //i=-1;
            }
            if (i == 3) {
                k = 0;
                i = -1;
            }
            i++;
        }
        PORTB = 0;
        RBIF = 0; //Desactivar bandera...

        //INTCONbits.RBIF==0;
    }

    if (TMR2IF == 1) // Timer has overflown
    {
        TMR2IF = 0; // Clear timer interrupt flag
        count++;
        Actuar();
    }
}
void Delay_ms(int x) {
    //__delay_ms(100);
    n = ' ';
    for (int i = 0; i < x; i++) {
        if (n == ' ') {
            __delay_ms(1);
        }
    }
    n = ' ';
}
void PIC_Configuracion_Inicial(void) {
    TRISD = 0;
    TRISCbits.TRISC3 = 1; //Entrada del detector de Humo
    TRISCbits.TRISC6 = 0; //Salida del detector de Humo (actuar)
    PORTCbits.RC6 = 0; //Salida del detector de Humo en 0 logico por defecto

    TRISCbits.TRISC0 = 0; //LCD_E
    TRISCbits.TRISC1 = 0; //LCD_RS
    
    TRISAbits.TRISA1 = 0; //Salida del indicador de Automatico(0 logico)/Manual(1 logico)
    PORTAbits.RA1 = 0; // Configuracion en modo automatico por defecto

    TRISB = 0b11110000;

    //ADCON1bits.PCFG = 0b111110; //A0 será la entrada analógica
    //Perifericos (Iniciar configuraciones analogicas)
    ADCON0bits.ADCS = 0b00; //ADCS1:ADCS0: A/D Conversion Clock Select bits 00 = FOSC/2    
    ADCON0bits.ADON = 1; //Encendemos el modulo analogico
    ADCON1bits.ADFM = 1; //Justificación a la derecha(De esta manera, los bits más significativos se almacenan en ADRESH)
    //ADCON0 = 0b00000000; //Configuracion del registro ADCON0
    /*INICIALIZACIÓN DE INTERRUPCIONES*/
    INTCON = 0; //limpiar registro INTCON
    
    ////////////////////////////////////////////////////////////////////////////
    /////////////////////////INTERUPCION POR TIEMPO/////////////////////////////
    T2CON = 0b00000100; // Timer0 with external freq and 32 as prescaler
    TMR2 = 0; // Load the time value for 1us delayValue can be between 0-256 only
    PR2 = 255;
    TMR2ON = 1; //Enable timer interrupt bit in PIE1 register
    TMR2IE = 1; //Enable timer interrupt bit in PIE1 register
    GIE = 1; //Enable Global Interrupt
    PEIE = 1; //Enable the Peripheral Interrupt
    /////////////////////////INTERUPCION POR TIEMPO/////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    
    ////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////PWM////////////////////////////////////////
    PR2 = 255;
    CCPR1L =    0b00011001;
    CCP1CON =   0b00111100; //    <5:4>   fin del paso 2
    //////////////////////Para Variar el Dcy se varia lo siguiente://////////////////////////////////////////
    CCPR1L =    0b10000000; // 50%
    CCP1CON =   0b00001100 ;////<1:3 = 11xx> colocar el bit 1:3 el 1 es para el modo PWM
    TRISCbits.TRISC2 = 1; // el pin CCP2 como entrada ( pwm apagado por defecto) 
    T1CON = 0b00000110; // Timer2 prescaler de 16
    /////////////////////////////////PWM////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    INTCONbits.GIE = 1; //Hab interrupciones Globales
    PORTB = 0;
    RBIF = 0;
    INTCONbits.RBIE = 1; //Port b

    OPTION_REGbits.nRBPU = 0;
}
void LCD_Escribir(char dato) {
    LCD_RS = 1;
    LCD_DATO = dato;
    LCD_E = 1;
    LCD_E = 0;
    __delay_us(40);
}

void LCD_Comando(char dato) {
    LCD_RS = 0;
    LCD_DATO = dato;
    LCD_E = 1;
    LCD_E = 0;
    if (dato <= 3) {
        __delay_ms(2);
    } else {
        __delay_us(40);
    }
}

void LCD_Configuracion_Inicial(void) {
    LCD_E = 0;
    __delay_ms(15);
    LCD_Comando(0x38);
    __delay_ms(5);
    LCD_Comando(0x38);
    __delay_ms(1);
    LCD_Comando(0x38);
    LCD_Comando(0x38);
    LCD_Comando(0x0C);
    LCD_Comando(0x01); //Display clear
    LCD_Comando(0x06); //incrementar el cursor

    LCD_Comando(0x0C); //DISPLAY ON/ CURSOR OFF /NO BLINKING

}

void LCD_EscribirStr(char str[]) {
    int i = 0;
    while (str[i] != '\0' && i < 1000) {
        LCD_Escribir(str[i]);
        i++;
    }
}

void LCD_Cursor(int h, int v) {
    char i;

    if (v == 1) {
        for (i = 0; i < h; i++) {
            __delay_us(40);
            LCD_Comando(0x14);
        }
    }

    if (v == 2) {
        h = h + sr;
        for (i = 0; i < h; i++) {
            __delay_us(40);
            LCD_Comando(0x14);
        }
    }

}

void LCD_Display(int Tam) {
    int aux1, i;

    __delay_ms(2);
    LCD_Comando(0x02);

    __delay_ms(300);
    if (Tam > 16 && Tam < sr) {
        aux1 = Tam - 16;
        n = ' ';
        for (i = -1; (i < aux1) && (n == ' '); i++) {
            __delay_ms(300);
            LCD_Comando(0x1B);
        }
    }
    if (n == ' ') {
        Delay_ms(600);
    }
    __delay_ms(2);
    LCD_Comando(0x02);
    __delay_ms(2);
}
