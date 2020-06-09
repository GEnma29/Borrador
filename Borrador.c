/*
 * File:   Interfaz.c
 * Author: guest-fb9e5db8-04cc-48b3-bfc3-9eec024147ed@transim.com
 *
 * Created on 5/26/2020 12:42:59 PM UTC
 * "Created in MPLAB Xpress"
 */


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

#define DHT11_PIN      PORTAbits.RA2
#define DHT11_PIN_DIR  TRISAbits.TRISA2

unsigned int Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
unsigned int sum, RH, TEMP;
unsigned int check = 0;

void Delay_ms(int x);

void LCD_Escribir(char dato);
void LCD_Comando(char dato);
void LCD_Configuracion_Inicial(void);
void PIC_Configuracion_Inicial(void);
void LCD_EscribirStr(char str[]);
void LCD_Cursor(int Horizontal, int Vertical);
void LCD_Display(int Tam);

void INTERFACE_Bienvenida(void);
void INTERFACE_Principal(void);
void INTERFACE_Datos(void);
void INTERFACE_Umbral(void);
void INTERFACE_Manual_Auto(void);

char DetectorMov(void);
void Fotocelda(void);
void Actuar(void);
void Umbral_Temp(void);
void Temperatura(void);
void Manual_Automatico(char x);

unsigned int ADC_Read(unsigned char canal);

unsigned char n = ' ', sr = 64;
unsigned char Q = 0;
unsigned int UT = 30, TEMP = 25;

unsigned int on_time; //= 150; //On-Time for the PWM signal
unsigned int count = 0; //count gets incremented for every timer overlap

void main(void) {

    PIC_Configuracion_Inicial();
    LCD_Configuracion_Inicial(); 

    //RBIE = 0;

    n = ' ';
    while (1) {
        INTERFACE_Bienvenida();
        Q = 1;
        while (Q == 1) {
            INTERFACE_Principal();
            //PORTAbits.RA1 = 0; // Configuracion en modo automatico por defecto
            
        }
    }
}
void Manual_Automatico(char x){
    if(x == 1){
    TMR2ON = 0; //Enable timer interrupt bit in PIE1 register
    TMR2IE = 0; //Enable timer interrupt bit in PIE1 register
    PEIE = 0; //Enable the Peripheral Interrupt
    }
    if(x == 0){
    TMR2ON = 1; //Enable timer interrupt bit in PIE1 register
    TMR2IE = 1; //Enable timer interrupt bit in PIE1 register
    PEIE = 1; //Enable the Peripheral Interrupt
    }   
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria DHT11///////////////////////////////////////////
void DHT11_Start(void)
{
	DHT11_PIN_DIR = 0;  // set the pin as output
	DHT11_PIN = 0;   // pull the pin low
	__delay_ms (18);  // wait for 18ms
	DHT11_PIN_DIR = 1;   // set the PIN as input
}

void DHT11_Check_Response(void)  // just ignore this but you have to put it in the main
{
	while(DHT11_PIN); // wait for pin to go low
    while(!DHT11_PIN); // wait for the pin to go HIGH
	while(DHT11_PIN); // wait for the pin to go low
}

unsigned int read_data (void)
{
	unsigned int i,j;
	for (j=0;j<8;j++)
	{
		while(DHT11_PIN == 0);   // wait for the pin to go high
		__delay_us (40);   // wait for 40 us
		if (DHT11_PIN == 0)   // if the pin is low 
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while(DHT11_PIN);  // wait for the pin to go low
	}
	return i;
}

void Temperatura(){
    /*int aux1, aux2;
    char UTAux = 0;
    n = ' ';
    while (UTAux == 0) {
        while (n == ' ') {
            LCD_Comando(0x02);
            LCD_EscribirStr("Temp: ");
            aux1 = TEMP / 10;
            LCD_Escribir(aux1 + '0');
            aux2 = TEMP % 10;
            LCD_Escribir(aux2 + '0');
        }
        switch (n) {
            case '2':
                TEMP = TEMP + 1;
                LCD_Comando(0x02);
                LCD_EscribirStr("Temp: ");
                aux1 = TEMP / 10;
                LCD_Escribir(aux1 + '0');
                aux2 = TEMP % 10;
                LCD_Escribir(aux2 + '0');
                n = ' ';
                break;
            case '8':
                TEMP = TEMP - 1;
                LCD_Comando(0x02);
                LCD_EscribirStr("Temp: ");
                aux1 = TEMP / 10;
                LCD_Escribir(aux1 + '0');
                aux2 = TEMP % 10;
                LCD_Escribir(aux2 + '0');
                n = ' ';
                break;
            case '#':
                UTAux = 1;
                break;
            default:
                n = ' ';
                break;
        }
    }*/
    LCD_EscribirStr("Initializing");
    for(int i=0; i<5; i++)
    {
        __delay_ms (500);
        LCD_EscribirStr('.');
        
    }
    __delay_ms (500);
    LCD_Comando(0x01);// Limpiar LCD 
    
    while(1)
    {
        DHT11_Start ();
		DHT11_Check_Response();
        
        // Read 40 bits (5 Bytes) of data
		Rh_byte1 = read_data();
		Rh_byte2 = read_data();
		Temp_byte1 = read_data();
		Temp_byte2 = read_data();
		sum = read_data();
        
        // print on LCD
        
        //LCD_Comando(0x80); // Comando
		LCD_EscribirStr("TEMP:- ");
		LCD_Escribir((Temp_byte1/10)+48);  // print 2nd digit
		LCD_Escribir(((Temp_byte1%10))+48);  // print 1st digit
		lLCD_EscribirStr(" C");
		
		LCD_Comando(0xC0);// comando 
		LCD_EscribirStr("RH:- ");
		LCD_Escribir((Rh_byte1/10)+48);  // print 2nd digit
		LCD_Escribir(((Rh_byte1%10))+48);  // print 1st digit
		LCD_EscribirStr(" %");
       
        
        __delay_ms(500);
    }
    return;
}

void Umbral_Temp(void) {
    int aux1, aux2;
    char UTAux = 0;
    n = ' ';
    while (UTAux == 0) {
        while (n == ' ') {
            LCD_Comando(0x02);
            LCD_Cursor(0, 2);
            LCD_EscribirStr("Temp: ");
            aux1 = UT / 10;
            LCD_Escribir(aux1 + '0');
            aux2 = UT % 10;
            LCD_Escribir(aux2 + '0');
        }
        switch (n) {
            case '2':
                UT = UT + 1;
                LCD_Comando(0x02);
                LCD_Cursor(0, 2);
                LCD_EscribirStr("Temp: ");
                aux1 = UT / 10;
                LCD_Escribir(aux1 + '0');
                aux2 = UT % 10;
                LCD_Escribir(aux2 + '0');
                n = ' ';
                break;
            case '8':
                UT = UT - 1;
                LCD_Comando(0x02);
                LCD_Cursor(0, 2);
                LCD_EscribirStr("Temp: ");
                aux1 = UT / 10;
                LCD_Escribir(aux1 + '0');
                aux2 = UT % 10;
                LCD_Escribir(aux2 + '0');
                n = ' ';
                break;
            case '#':
                UTAux = 1;
                break;
            default:
                n = ' ';
                break;
        }
    }
}

///////////////////////////////////////////Libreria DHT11///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria INTERFACE///////////////////////////////////////

void INTERFACE_Manual_Auto(void) {
    int w, x, y, z;

    n = ' ';
    w = 0;
    x = 0;
    y = 0;
    z = 0;

    LCD_Comando(0x01); // Limpiar Display
    LCD_EscribirStr(" 1: Manual");
    LCD_Cursor(-10, 2);
    LCD_EscribirStr(" 2: Automatico (defecto)");
    /*LCD_Comando(0x02); //regresar cursor a casa
    __delay_ms(1.64);
    LCD_Cursor(13, 1);
    LCD_EscribirStr("#3: Menu Principal");*/
    LCD_Display(23);
    while (y == 0) {
        while (n == ' ') {
        }
        switch (n) {
            case'1':
                n = ' ';
                LCD_Comando(0x01); // Limpiar Display
                LCD_Cursor(2, 1);
                LCD_EscribirStr("AjusteManual");
                PORTAbits.RA1 = 1;
                LCD_Cursor(-14, 2);
                LCD_EscribirStr("1: Alarma 2: Luz ");
                LCD_Display(26);
                Manual_Automatico(1);
                y = 1;
                while (w == 0) {
                    while (n == ' ') {
                    }
                    switch (n) {
                        case'1':
                            n = ' ';
                            LCD_Comando(0x01); // Limpiar Display
                            LCD_EscribirStr("    Alarma:");
                            LCD_Comando(0x02);
                            LCD_Cursor(0,2);
                            LCD_EscribirStr("1: On   2: Off");
                            w = 1;
                            n = ' ';
                            while (n == ' ') {
                            }
                            while (z == 0) {
                                while (n == ' ') {
                                }
                                switch (n) {
                                    case'1':
                                        n = ' ';
                                        LCD_Comando(0x01); // Limpiar Display
                                        LCD_EscribirStr("   Alarma On");
                                        TRISCbits.RC6 = 0; // entrada del sensor
                                        Delay_ms(3000);
                                        z = 1;
                                        n = ' ';
                                        break;
                                    case'2':
                                        n = ' ';
                                        LCD_Comando(0x01); // Limpiar Display
                                        LCD_EscribirStr("   Alarma Off");
                                        PORTCbits.RC6 = 0; //Salida del pir 0 logico por defecto
                                        Delay_ms(3000);
                                        z = 1;
                                        n = ' ';
                                        break;
                                    case'#': // Menu principal
                                        LCD_Comando(0x01); // Limpiar Display
                                        z = 1;
                                        n = ' ';
                                        break;
                                    default:
                                        z = 0;
                                        n = ' ';
                                        break;

                                }
                            }//aqui muere el switch 3 luminaria
                            break;
                        case'2':
                            n = ' ';
                            LCD_Comando(0x01); // Limpiar Display
                            LCD_EscribirStr("  Ventanas:");
                            LCD_Cursor(-14, 2);
                            LCD_EscribirStr("1: Open   2: Close");
                            w = 1;
                            n = ' ';
                            while (n == ' ') {
                            }
                            while (x == 0) {
                                while (n == ' ') {
                                }
                                switch (n) {
                                    case'1':
                                        n = ' ';
                                        LCD_Comando(0x01); // Limpiar Display
                                        LCD_EscribirStr(" Ventanas Open");
                                        TRISCbits.TRISC2 = 0; // el pin CCP2 como entrada ( pwm apagado por defecto) 
                                        Delay_ms(3000);
                                        x = 1;
                                        n = ' ';
                                        break;
                                    case'2':
                                        n = ' ';
                                        LCD_Comando(0x01); // Limpiar Display
                                        LCD_EscribirStr(" Ventilacion Close");
                                        TRISCbits.TRISC2 = 1; // el pin CCP2 como entrada ( pwm apagado por defecto) 
                                        Delay_ms(3000);
                                        x = 1;
                                        n = ' ';
                                        break;
                                    case'#': // Menu principal
                                        LCD_Comando(0x01); // Limpiar Display
                                        x = 1;
                                        n = ' ';
                                        break;
                                    default:
                                        x = 0;
                                        n = ' ';
                                        break;
                                }//aqui muere el switch 4 ventilacion
                            }
                            break;
                        case'#': // Menu principal
                            LCD_Comando(0x01); // Limpiar Display
                            w = 1;
                            n = ' ';
                            break;
                        default:
                            w = 0;
                            n = ' ';
                            break;
                    }//aqui muere el switch 2 - Ajuste manual, luminaria o ventilacion?
                }
                break;
            case'2': // Automatico
                n = ' ';
                LCD_Comando(0x01); // Limpiar Display
                LCD_EscribirStr("   AjusteAuto");
                PORTAbits.RA1 = 0;
                LCD_Cursor(-13, 2);
                LCD_EscribirStr("Conf por defecto");
                Manual_Automatico(0);
                y = 1;
                n = ' ';
                Delay_ms(3000);
                break;
            case'#': // Menu principal
                LCD_Comando(0x01); // Limpiar Display
                y = 1;
                n = ' ';
                break;
            default:
                y = 0;
                n = ' ';
                break;
        }
    }
}

void INTERFACE_Umbral(void) {
    char y;

    n = ' ';
    y = 0;

    LCD_Comando(0x01); // Limpiar Display
    LCD_EscribirStr("Umbrales de actuadores:");
    LCD_Cursor(-23, 2);
    LCD_EscribirStr(" 1: Temp  2: Por defecto");
    LCD_Display(24);
    while (y == 0) {
        while (n == ' ') {
        }
        switch (n) {
            case'1':
                n = ' ';
                LCD_Comando(0x01); // Limpiar Display
                LCD_EscribirStr("  Umbral Temp.");
                LCD_Comando(0x02);
                LCD_Cursor(0,2);
                Umbral_Temp();
                Delay_ms(3000);
                y = 1;
                n = ' ';
                break;
            case'2':
                n = ' ';
                LCD_Comando(0x01); // Limpiar Display
                LCD_EscribirStr(" Umbral Defecto");
                UT = 30;
                LCD_Comando(0x02);
                LCD_Cursor(0,2);
                LCD_EscribirStr("    Temp. 30C   ");
                Delay_ms(3000);
                y = 1;
                n = ' ';
                break;
            case'#':
                LCD_Comando(0x01); // Limpiar Display
                y = 1;
                break;
            default:
                y = 0;
                n = ' ';
                break;
        }
    }
}

void INTERFACE_Datos(void) {
    char y;

    n = ' ';
    y = 0;

    LCD_Comando(0x01); // Limpiar Display
    LCD_EscribirStr("1: Temperatura");
    LCD_Comando(0x02);
    LCD_Cursor(0,2);
    LCD_EscribirStr("2: Presencia ");
    LCD_EscribirStr("3: Lumenes");
    LCD_Display(19);
    while (y == 0) {
        while (n == ' ') {
        }
        switch (n) {
            case'1':
                LCD_Comando(0x01); // Limpiar Display
                LCD_Comando(0x02);
                LCD_Cursor(0,2);
                LCD_EscribirStr("Humedad: 36%");
                Temperatura();
                Delay_ms(3000);
                y = 1;
                break;
            case'2':
                LCD_Comando(0x01); // Limpiar Display
                LCD_EscribirStr("Nivel de CO: 15%");
                LCD_Cursor(-16, 2);
                Delay_ms(3000);
                y = 1;
                break;
            case'3':
                LCD_Comando(0x01); // Limpiar Display
                Fotocelda();
                //__delay_ms(5000);
                y = 1;
                break;
            case'#':
                LCD_Comando(0x01); // Limpiar Display
                y = 1;
                n = ' ';
                break;
            default:
                y = 0;
                n = ' ';
                break;
        }
    }
}

void INTERFACE_Principal(void) {
    char y;

    n = ' ';
    y = 0;

    LCD_Comando(0x01); // Limpiar Display
    LCD_EscribirStr(" 1: Datos");
    LCD_Cursor(3, 1);
    LCD_EscribirStr(" 2: Umbral");
    LCD_Cursor(-22, 2);
    LCD_EscribirStr(" 3: Man / Auto");
    LCD_Display(22);
    while (y == 0) {
        while (n == ' ') {
        }
        switch (n) {
            case'1':
                INTERFACE_Datos();
                y = 1;
                n = ' ';
                break;
            case'2':
                INTERFACE_Umbral();
                y = 1;
                n = ' ';
                break;
            case'3':
                INTERFACE_Manual_Auto();
                y = 1;
                n = ' ';
                break;
            case'#':
                Q = 0;
                y = 1;
                n = ' ';
                break;
            default:
                y = 0;
                n = ' ';
                break;
        }
    }
}

void INTERFACE_Bienvenida(void) {

    char t = 3, i = 1, x;
 
    LCD_Comando(0x01); // Limpiar Display
    LCD_Cursor(3, 1); // 3---> Numero de cuadros del cursor que se desea desplazar hacia la derecha - 63 la primera lina, para la simulacion - 40 la primera linea para el montaje
    // 1---> Arriba, 2 Abajo
    LCD_EscribirStr("SmartVite");

    char j = '0';
    n = ' ';
    for (i = 1; (i <= t)&&(n == ' '); i++) {

        LCD_Comando(0x02); //regresar cursor a casa
        LCD_Cursor(8, 2);
        
        j = i + '0';
        LCD_Escribir(j);
        
        
        for (x = 0; (x < 100)&&(n == ' '); x++) {
            __delay_ms(5);
        }
    }
    //Delay_ms(500);
    LCD_Comando(0x01); // Limpiar Display
    n = ' ';
}
///////////////////////////////////////////Libreria INTERFACE///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Libreria LCD/////////////////////////////////////////////

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
///////////////////////////////////////////Libreria LCD/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Actuar() {
    if (DetectorMov() == 1) {
        PORTCbits.RC6 = 1;
    } else {
        PORTCbits.RC6 = 0;
    }
    
    if(TEMP>=UT){
        TRISCbits.TRISC2 = 0; //PWM activado
    }else{
        TRISCbits.TRISC2 = 1; //PWM desactivado
    }
}

char DetectorMov() {
    TRISCbits.TRISC3 = 1;
    //TRISCbits.TRISC4 = 0;
    //PORTCbits.RC4 = 0;
    if (PORTCbits.RC3 == 1) {
        return 0;
    } else {
        return 1;
    }
}

void Fotocelda() {

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

        LCD_EscribirStr("Flujo luminoso:");
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
        LCD_Comando(0x02);
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
