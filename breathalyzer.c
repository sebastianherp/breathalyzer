#pragma config OSC = INTIO2
#pragma config PWRT = ON 
#pragma config BOR = OFF 
#pragma config WDT = OFF  //Watchdog Timer 
#pragma config MCLRE = OFF

#include <p18cxxx.h>
#include <math.h>

void setup(void);
char adc2leds(int value);
void led(char nr, char on);
void delay(unsigned int timeout);
int measure(void);

#define LEDCOUNT 10

char adc2leds(int value) {
	if(value < 615) return 0; // < 0,2 Promille
	if(value < 680) return 1; // < 0,3 Promille
	if(value < 721) return 2; // < 0,4
	if(value < 753) return 3; // < 0,5
	if(value < 777) return 4; // < 0,6
	if(value < 811) return 5; // < 0,8
	if(value < 835) return 6; // < 1,0
	if(value < 853) return 7; // < 1,2
	if(value < 873) return 8; // < 1,5
	if(value < 888) return 9; // < 1,8
	return 10; // > 1,8
}

void led(char nr, char on) {
	switch(nr) {
	case 0: PORTBbits.RB1 = on; break;
	case 1: PORTBbits.RB0 = on; break;
	case 2: PORTAbits.RA3 = on; break;
	case 3: PORTAbits.RA2 = on; break;
	case 4: PORTAbits.RA4 = on; break;
	case 5: PORTAbits.RA1 = on; break;
	case 6: PORTAbits.RA6 = on; break;
	case 7: PORTAbits.RA7 = on; break;
	case 8: PORTBbits.RB2 = on; break;
	case 9: PORTBbits.RB3 = on; break;
	}
}

// in milliseconds
void delay(unsigned int timeout) {
	char i;
	while(timeout-- > 0) {
		i = 14;
		while(i-- > 0);
	}
}

int measure(void) {
	// switch on A/D before calling this!

	ADCON0bits.GO = 1;
	while(ADCON0bits.DONE);
	return (int)ADRESH<<8 | ADRESL;
}

void setup(void) {
	char i;
	ADCON1 = 0x0F; // ports digital
    PORTA = 0;
    PORTB = 0;
	
    // Set RA4 as input and RB3-RB0 as output
    TRISA = 0b00000001;
    TRISB = 0b00010000;

	//AN0 on
	ADCON1 &= 0b11111110;
	ADCON0 = 0b00000000;
	ADCON2 = 0b10001010; // 2 TAD acquisition time & FOSC/32 conversion clock
	ADCON0 |= 1; // turns adc on

	for(i=0;i<LEDCOUNT;i++) {
		led(i, 1);
		delay(40);
	}	
	for(i=LEDCOUNT-1;i>=0;i--) {
		led(i, 0);
		delay(40);
	}	
}

int value = 0;
int value_avg = 0;
int value_max = 0;
int timer = 0;
int leds = 0;

void main(void)
{

	char i;

	setup();

	while(1) {
		delay(100);

		value = measure();
		value_avg = value; //(value_avg * 15 + value) >> 4;
		if(value_avg > value_max) {
			value_max = value_avg;
			timer = 100;
		}
		if(timer > 0) {
			timer--;
		} else {
			value_max = value_avg;
		}

		if(PORTBbits.RB4) // pin high -> promille
			leds = adc2leds(value_max);
		else // pin low -> adc
			leds = (value-523)/50;

		if(leds > LEDCOUNT) leds = LEDCOUNT;

		for(i=0;i<leds;i++) {
			led(i, 1);
		}	
		for(i=leds;i<LEDCOUNT;i++) {
			led(i, 0);
		}	
	}
}