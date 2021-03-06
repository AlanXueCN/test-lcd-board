
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "main.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "pong.h"
#include "tetris.h"

uint8_t debug = 0; //DEBUGING INFORMATIONEN -> enum
uint8_t lcd_optimized =0; //HAS to be TRUE from the beginning

enum displ_orientation ausrichtung;
	
void wait_for_ok(void);

int main (void)
{

	ausrichtung = unten;
	
	
	// enable pull ups for the 4 keys
	PORTA |= (1<<PORTA4)|(1<<PORTA5)|(1<<PORTA6)|(1<<PORTA7);

    // disable bootloader LED
    DDRC |= (1<<DDC3);
	PORTC |= (1<<PORTC3);
            
	
	// set LED Pins to output (are low per default, so active)
	DDRC |= (1<<DDC0)|(1<<DDC1)|(1<<DDC2); 
	DDRD |= (1<<DDD6)|(1<<DDD7); //LCD Display LED DDD6(li), DDD7(mi), DDC2(re) anschalten



	LCD_Init ();
	USART_Init ();
	TIMER0_Init ();
	sei ();

	lcd_cls ();
if (debug) {lcd_line (0,0,127,63,1); wait_ms(3000);	lcd_cls();}
	
int auswahl;
auswahl = 1;
	while(1)
	{
	ausrichtung = unten;
	
//lcd_write_number_u(ausrichtung);
//wait_ms(5000);

	
		lcd_printp_at(5,0,PSTR("1: opt=0\r\n"),0);
		lcd_printp_at(5,10,PSTR("2: opt=1\r\n"),0);
		lcd_printp_at(5,30,PSTR("Auswahl: "),0);
		lcd_write_number_u(auswahl);
		
		//wait_ms(100);
		if(get_key_press(1 << KEY_MINUS)) {
			lcd_cls();
			lcd_printp_at(0,0,PSTR("ready"),0);
			asm volatile("JMP 0xe000");
		}
		if(get_key_press(1 << KEY_PLUS)) {
			if (auswahl < 2) auswahl++;
			}
		if(get_key_press(1 << KEY_ESC)) {
			if (auswahl > 1) auswahl--;
			}
		if(get_key_press(1 << KEY_ENTER)) {
		switch(auswahl) {
			case(1):
				lcd_cls();
				lcd_optimized = 0;
				tetris();
				break;
			case(2) :
				lcd_cls();
				lcd_optimized = 1;
				tetris();
				lcd_optimized = 1;
				break;
			}
		}

	}

}
