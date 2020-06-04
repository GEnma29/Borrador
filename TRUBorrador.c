/*
 * File:   newmain.c
 * Author: guest-d35b4891-8ac2-4080-9a2b-bac4482cdfcf@transim.com
 *
 * Created on 6/4/2020 3:16:48 PM UTC
 * "Created in MPLAB Xpress"
 */


#include <stdio.h>
#include <xc.h>


#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = ON         // FLASH Program Memory Write Enable (Unprotected program memory may be written to by EECON control)

#define _XTAL_FREQ 4000000

#define LCD_RS PORTCbits.RC1
#define LCD_E PORTCbits.RC0
#define LCD_DATO PORTD

#define _XTAL_FREQ 4000000

#define LCD_RS PORTCbits.RC1
#define LCD_E PORTCbits.RC0
#define LCD_DATO PORTD

unsigned int ADC_Read(unsigned char canal);

//unsigned char n = ' ', sr = 64;
//unsigned char Q = 0;
//unsigned int UT = 30, TEMP = 25;

unsigned int on_time; //= 150; //On-Time for the PWM signal
unsigned int count = 0; //count gets incremented for every timer overlap


void LCD_Escribir(char dato);
void LCD_Cmd(char dato);
void Presencia_salir();
void LCD_Configuracion_Inicial(void);
void PIC_Configuracion_Inicial(void);
void LCD_Escribir_Cadena(char str[]);
void LCD_Cursor(int Horizontal, int Vertical);
void LCD_Display(int Tam);
int LCD_Contar(char c[]);
void ClosePersianas();
void Intro();
void Delay_2s();
void AperturaDepercians();
void Timer0Delay(void);
void Retardo_2segundos();
void Visualisar_Config();
void Temperatura_Humedad();
void Retardo_1segundos();
void Humedad();
void Fotocelda();
void OpenPersianas();
void Temperatura();
void Luz_Natural();
void Luz_Na_Ar();
void Actuar();
void Luz_02_Artificial();
void Luz_03_Artificial_Manual();
void Luz_03_Artificial_Automatica();
void AbrirPercianas();
void AperturaHig();
void Aperturamedia();
void Aperturalow();
void Delay_ms(int x);
unsigned char n;
unsigned int Status,Variable,j,l,g;
unsigned char n = ' ';
unsigned int a = 0, Q = 0;
unsigned char selector1,selector2,selector3,selector4,selector5;

void main(void) {
    char aux0[] = "0";
    int selector1 = 0;
    a = 0;

    PIC_Configuracion_Inicial();
    LCD_Configuracion_Inicial();
    PORTAbits.RA1 = 0; // Configuracion en modo automatico

    //RBIE = 0;
    Intro();
    Retardo_2segundos();
    Visualisar_Config();
    Retardo_2segundos();
    while(1){
    switch(n){
        if((n == '1')&&(a == 0)){
           n = '1'; 
        }
        if((n == '2')&&( a == 0)){
            n = '2';
        }
        case '1':
             n = ' ';
             a = 1;
            Temperatura_Humedad(); 
            Retardo_2segundos();
            if((n == '1')&&(a == 1)){
            selector1 = '1';
            n = ' ';
            }
            if((n == '2')&&(a == 1)){
            selector1 = '2';
            n = ' ';
            }
            
            switch(selector1){
                case '1':
                    selector1 = ' ';
                    Temperatura();
                    Delay_2s();
                    a = 0;
                    break;
                case '2':
                    selector1 = ' ';
                    Humedad();
                    a = 0;
                    break;
                    
                case '3':
                    selector1 = ' ';
                    Fotocelda();
                    Delay_ms(3000);
                    a = 0;
                    break;
                
                    break;
            
            }
            
            break;
        
        case '2':
             n = ' ';
             selector2 = '0';
             
            Luz_Na_Ar();
            Retardo_2segundos();
            if(n == '1'){
            selector2 = 'a';
            n = ' ';
            a = 2;
            }
            if(n == '2'){
            selector2 = 'b';
            n = ' ';
            a = 2;
            }
            if(n == '3'){
            selector2 = 'c';
            n = ' ';
            a = 2;
            }
            if(n == '4'){
            selector2 = 'd';
            n = ' ';
            a = 2;
            }
            
            switch(selector2){
                case 'a':
                    selector2 = 'z';
                    Luz_Natural();
                    Retardo_1segundos();
                    if((n =='1')&&(selector2 == 'z')){
                        selector3 = '1';
                    }
                    if((n == '2')&&(selector2 == 'z')){
                        selector3 = '2';
                    }
                    switch(selector3){
                        case '1':
                            selector3 = 'o';
                            AbrirPercianas();
                            Retardo_1segundos();
                            
                            if((n == '1')&&(selector3 == 'o')){
                                selector4 = '1';
                            }
                            if((n == '1')&&(selector3 == 'o')){
                                selector4 = '2';
                            }
                            switch(selector4){
                                case '1':
                                    a = 4;
                                    if((n == '1' )&&(a==4)){
                                        selector5 = '1';
                                    }
                                    if((n == '2' )&&(a==4)){
                                        selector5 = '2';
                                    }
                                    if((n == '3' )&&(a==4)){
                                        selector5 = '3';
                                    }
                                    
                                    OpenPersianas();
                                    switch(selector5){
                                        case '1':
                                             Aperturalow();
                                             Retardo_1segundos();
                                            break;
                                        case '2':
                                            Aperturamedia();
                                            Retardo_1segundos();
                                            
                                            break;
                                        case '3':
                                            AperturaHig();
                                            Retardo_1segundos(); 
                                            break;
                                            
                                    
                                    
                                    }
                                    break;
                                case '2':
                                    ClosePersianas();
                                    break;
                            
                            }
                            
                            break;
                        case '2':
                            Luz_03_Artificial_Automatica();
                            Retardo_1segundos();
                            break;
                    
                    }
                    break;
                case 'b':
                    selector2 = ' ';
                    Humedad();
                    
                    break;
                    
                case 'c':
                    break;
                    
                case 'd':
                    break;
            
            
            break;
            
        case '3':
            break;
        //default:LCD_Escribir_Cadena("No chamo que va ");
            
    
            
  
    }
             
}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria INTERFACE///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria LCD/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Escribir(char dato){
    LCD_RS = 1;
    LCD_DATO = dato;
    LCD_E = 1;
    LCD_E = 0;
    __delay_us(40);
}

void LCD_Cmd(char dato) {
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
    LCD_Cmd(0x38);
    __delay_ms(5);
    LCD_Cmd(0x38);
    __delay_ms(1);
    LCD_Cmd(0x38);
    LCD_Cmd(0x38);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x01); //Display clear
    LCD_Cmd(0x06); //incrementar el cursor

    LCD_Cmd(0x0C); //DISPLAY ON/ CURSOR OFF /NO BLINKING

}

void LCD_Escribir_Cadena(char str[]) {
    int i = 0;
    while (str[i] != '\0' && i < 1000) {
        LCD_Escribir(str[i]);
        i++;
    }
}

void LCD_Cursor(int h, int v) {
    int i;

    if (v == 1) {
        for (i = 0; i < h; i++) {
            __delay_us(40);
            LCD_Cmd(0x14);
        }
    }

    if (v == 2) {
        h = h + 64;
        for (i = 0; i < h; i++) {
            __delay_us(40);
            LCD_Cmd(0x14);
        }
    }

}

int LCD_Contar(char c[]) {
    int i = 0;
    while (c[i] != '\0' && i < 1000) {
        i++;
    }
    return i;
}

void LCD_Display(int Tam) {
    int aux1, i;
    __delay_ms(2);
    LCD_Cmd(0x02);

    __delay_ms(1000);
    if (Tam > 16 && Tam < 64) {
        aux1 = Tam - 16;
        for (i = -1; i < aux1; i++) {
            __delay_ms(300);
            LCD_Cmd(0x1B);
        }
    }
    __delay_ms(2);
    LCD_Cmd(0x02);
    __delay_ms(2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria LCD/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void PIC_Configuracion_Inicial(void) {
    TRISD = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISA = 0;
    TRISB = 0b11110000;
    ADCON0 = 0b00000000; //Configuracion del registro ADCON0
    /*INICIALIZACIÓN DE INTERRUPCIONES*/
    INTCON = 0; //limpiar registro INTCON
    INTCONbits.GIE = 1; //Hab interrupciones Globales
    PORTB = 0;
    RBIF = 0;
    INTCONbits.RBIE = 1; //Port b

    OPTION_REGbits.nRBPU = 0;
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
       // Actuar();
    }
}
void Intro(){
 
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Welcome to");
    LCD_Cursor(0,0);
    LCD_Cursor(-6,2);
    LCD_Escribir_Cadena("Smartvite");
}
void Visualisar_Config(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("1.visualizar");
    LCD_Cursor(0,0);
    LCD_Cursor(-12,2);
    LCD_Escribir_Cadena("2.configurar");
}
void Temperatura_Humedad(){
     LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("1.Temperatura");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("2.Humedad 3.Luz");
}
void Luz_Na_Ar(){
     LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("1.Luz Natural");
    LCD_Cursor(-14,2);
    LCD_Escribir_Cadena("2.Luz Artificial");
   
}
void AbrirPercianas(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Escribir_Cadena("1.OPEN");
    LCD_Cursor(-15,2);
    LCD_Escribir_Cadena("2.CLOSE");

}
void Presencia(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("3.Presencia");
    LCD_Cursor(-7,2);
    LCD_Escribir_Cadena("4.Salir");
}
void Luz_02_Artificial(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("1.Automatica");
    LCD_Cursor(-8,2);
    LCD_Escribir_Cadena("2.manual");
}
void Luz_03_Artificial_Manual(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las luces han sido ");
     LCD_Cursor(-8,2);
    LCD_Escribir_Cadena("han sido encendidas");
}
void Luz_03_Artificial_Automatica(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las luces seran ");
     LCD_Cursor(-19,2);
    LCD_Escribir_Cadena("encendidas a las 6 pm");
}
void Temperatura(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("La Temperatura ");
    LCD_Cursor(-12,2);
    LCD_Escribir_Cadena("26 C");
}
void Humedad(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("La Humedad es:");
    LCD_Cursor(-10,2);
    LCD_Escribir_Cadena("68%");

}
void Luz_Natural(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(0,1);
    LCD_Escribir_Cadena("1.abrir");
    LCD_Cursor(-7,2);
    LCD_Escribir_Cadena("2.cerrar");
}
void Presencia_salir(){
LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("3.Precensia");
    LCD_Cursor(-11,2);
    LCD_Escribir_Cadena("4.Salir");

}
void Retardo_2segundos(){
    int i;
    for(i=0; i<20;i++){
        __delay_ms(100);
    }
}
void Retardo_1segundos(){
    int i;
    for(i=0; i<10;i++){
        __delay_ms(100);
    }
}
void ClosePersianas(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las percianas han");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("sido cerradas");
}
void OpenPersianas(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las percianas han");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("sido abiertas");
}
void AperturaDepercians(){


}
void Fotocelda(){

    n = ' ';
    int valor;
    long lux1;
    int uni, dec, cen, mil;

    while (n == ' ') {
        valor = ADC_Read(0); //Se coloca 0 porque es el canal analogico RA0

        if (valor > 725) {
            lux1 = (-6 * valor + 5010) / 10;

            if (valor > 835)
                lux1 = 0;
        } else {
            lux1 = ((8 * valor / 1000 - 11600 / 1000) * valor) + 4271;
        }

        LCD_Escribir_Cadena("Flujo luminoso:");
        LCD_Cursor(-15, 2);

        if (lux1 < 0) {
            lux1 = -lux1;
        }

        mil = (lux1) / 1000;
        LCD_Escribir('0' + mil);
        cen = (lux1 - mil * 1000) / 100;
        LCD_Escribir('0' + cen);
        dec = (lux1 - mil * 1000 - cen * 100) / 10; // a partir de aqui es el error
        LCD_Escribir('0' + dec);
        uni = lux1 % 10;
        LCD_Escribir('0' + uni);


        __delay_ms(500);
         LCD_Cmd(0x01); // Limpiar Display
    }
}

unsigned int ADC_Read(unsigned char canal) {
    ADCON0 &= 0xC5; //Valor para reiniciar los canales a 0
    ADCON0bits.CHS = canal; //Se elige el canal
    __delay_ms(2); //Espera para el cambio
    GO_nDONE = 1; //Se activa la
    while (GO_nDONE); //Esperamos hasta que terminen
    int X = ADRESH << 8;
    X = X + ADRESL; //Los valores se guardarán en el registro ADRES H(2 mas significativos) L(6 menos significativos)
    return (X);

}

void Delay_2s(){
    int i;
    for(i = 0; i < 200; i++){//Toggle Output
        if(n != 0){
            i=200; //Delay
        }       
    }


}
void Aperturalow(){
    LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las apertura es de ");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("23");
}
void Aperturamedia(){
       LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las apertura es de ");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("56");
}
void Delay_ms(int x){
    //__delay_ms(100);
    n = ' ';
    for (int i = 0; i < x; i++) {
        if (n == ' ') {
            __delay_ms(1);
        }
    }
    n = ' ';
}
void AperturaHig(){
       LCD_Cmd(0x01); // Limpiar Display
    LCD_Cursor(1,1);
    LCD_Escribir_Cadena("Las apertura es de ");
    LCD_Cursor(-13,2);
    LCD_Escribir_Cadena("76");
}
void Timer0Delay(void){     //10ms delay
    T1CON=0x01;             //Timer-1 16-bit mode Prescaler 1:4
    TMR1H=0x30;             //Count Hight Byte
    TMR1L=0xD4;             //Count Low Byte
   
    T1CONbits.TMR1ON=1;              //Run timer
    while(INTCONbits.TMR0IF==0);     //Wait for flag to over flow
    T1CONbits.TMR1ON=0;              //Switch off timer
    INTCONbits.TMR0IF=0;             //Clear Interrupt
   
}
//void Actuar() {
  /*  if (DetectorMov() == 1) {
        PORTCbits.RC6 = 1;
    } else {
        PORTCbits.RC6 = 0;
    }
    
    if(TEMP>=UT){
        TRISCbits.TRISC2 = 0; //PWM activado
    }else{
        TRISCbits.TRISC2 = 1; //PWM desactivado
    }*/
//}
