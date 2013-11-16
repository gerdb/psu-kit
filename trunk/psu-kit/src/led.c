/*
 *  Project:      psu-kit
 *  File:         led.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  LED display control
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <avr/io.h>
#include "project.h"
#include "led.h"

/*
 * local functions
 */

/*
 * local variables
 */
unsigned char digit[6] = {' ',' ',' ',' ',' ',' '};
unsigned char led_mux = 0;
unsigned char digit_blink[2] = {0,0};
unsigned char blink = 0;
unsigned char blink_cnt = 0;

// LED font
const unsigned char led_font[48] = {
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F		 ,	// LED character: "0"
			SEG_B | SEG_C								 ,	// LED character: "1"
	SEG_A | SEG_B |      	SEG_D | SEG_E |      	SEG_G,	// LED character: "2"
	SEG_A | SEG_B | SEG_C | SEG_D |            		SEG_G,	// LED character: "3"
			SEG_B | SEG_C |            		SEG_F | SEG_G,	// LED character: "4"
	SEG_A |      	SEG_C | SEG_D |      	SEG_F | SEG_G, 	// LED character: "5"
	SEG_A |      	SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "6"
	SEG_A | SEG_B | SEG_C								 ,	// LED character: "7"
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "8"
	SEG_A | SEG_B | SEG_C | SEG_D |      	SEG_F | SEG_G, 	// LED character: "9"
	0													 ,	// LED character: ":"
	0													 ,  // LED character: ";"
	SEG_A |                        			SEG_F | SEG_G, 	// LED character: "<"
							SEG_D |      			SEG_G,	// LED character: "="
					SEG_C | SEG_D | 				SEG_G,	// LED character: ">"
	SEG_A | SEG_B |            		SEG_E |      	SEG_G,	// LED character: "?"
	0                                        			 ,	// LED character: "@"
	SEG_A | SEG_B | SEG_C |      	SEG_E | SEG_F | SEG_G, 	// LED character: "A"
					SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "b"
							SEG_D | SEG_E |      	SEG_G,	// LED character: "c"
			SEG_B | SEG_C | SEG_D | SEG_E |      	SEG_G,	// LED character: "d"
	SEG_A |           		SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "E"
	SEG_A |                  		SEG_E | SEG_F | SEG_G,  // LED character: "f"
	SEG_A | SEG_B | SEG_C | SEG_D |			SEG_F | SEG_G, 	// LED character: "g"
					SEG_C |      	SEG_E | SEG_F | SEG_G,	// LED character: "h"
					SEG_C            					 ,	// LED character: "i"
	SEG_A | SEG_B | SEG_C | SEG_D                  		 ,	// LED character: "J"
							SEG_D |      	SEG_F | SEG_G,	// LED character: "k"
							SEG_D | SEG_E | SEG_F      	 ,	// LED character: "L"
	SEG_A | SEG_B | SEG_C |      	SEG_E | SEG_F      	 ,	// LED character: "M"
					SEG_C |      	SEG_E |      	SEG_G,	// LED character: "n"
					SEG_C | SEG_D | SEG_E |      	SEG_G,	// LED character: "o"
	SEG_A | SEG_B |            		SEG_E | SEG_F | SEG_G,	// LED character: "P"
	SEG_A | SEG_B | SEG_C |            		SEG_F | SEG_G,	// LED character: "q"
									SEG_E |      	SEG_G,	// LED character: "r"
	SEG_A |      	SEG_C | SEG_D |      	SEG_F | SEG_G,	// LED character: "S"
							SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "t"
					SEG_C | SEG_D | SEG_E            	 ,	// LED character: "u"
							SEG_D | SEG_E            	 ,	// LED character: "v"
			SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// LED character: "W"
			SEG_B | SEG_C |     	SEG_E | SEG_F | SEG_G,	// LED character: "X"
			SEG_B | SEG_C | SEG_D |			SEG_F | SEG_G,	// LED character: "Y"
	SEG_A | SEG_B |      	SEG_D | SEG_E |      	SEG_G,	// LED character: "Z"
			SEG_B | 				SEG_E | 		SEG_G,	// LED character: "["
	0													 ,	// LED character: "\"
					SEG_C | 				SEG_F | SEG_G,  // LED character: "]"
	SEG_A												 , 	// LED character: "^"
							SEG_D              				// LED character: "_"
};


/*
 * Initialize the LED module
 */
void LED_Init(void) {

	// LED Ports as output
	DDRD  = 0b11111111;
	DDRB |= 0b11011001;
}

/*
 * Switch off all 6 digits
 */
void LED_DigitsOff(void) {
#ifdef DISPLAY_LED_CC
	PORTD |= 0b11100000;
	PORTB |= 0b00011001;
#endif

#ifdef DISPLAY_LED_CA
	PORTD &= ~0b11100000;
	PORTB &= ~0b00011001;
#endif
}

/*
 * Switch off all segments
 */
void LED_SegmentsOff(void) {

#ifdef DISPLAY_LED_CC
	PORTD &= ~0b00011111;
	PORTB &= ~0b11000000;
#endif

#ifdef DISPLAY_LED_CA
	PORTD |= 0b00011111;
	PORTB |= 0b11000000;
#endif

}

/*
 * Switch on segments
 */
void LED_SegmentsOn(unsigned char c) {
	unsigned char led_c;

	led_c = led_font[c-'0'];

#ifdef DISPLAY_LED_CC
	PORTD |= (led_c & 0b00011111);
	PORTB |= (led_c & 0b11000000);
#endif

#ifdef DISPLAY_LED_CA
	PORTD &= ~(led_c & 0b00011111);
	PORTB &= ~(led_c & 0b11000000);
#endif

}


/*
 * Switch off one digit
 */
void LED_DigitOn(unsigned char digit) {
	switch (digit) {

#ifdef DISPLAY_LED_CC
	case 0: PORTD &= ~_BV(PB5); break;
	case 1: PORTD &= ~_BV(PB6); break;
	case 2: PORTD &= ~_BV(PB7); break;
	case 3: PORTB &= ~_BV(PB0); break;
	case 4: PORTB &= ~_BV(PB3); break;
	case 5: PORTB &= ~_BV(PB4); break;
#endif

#ifdef DISPLAY_LED_CA
	case 0: PORTD |= _BV(PB5); break;
	case 1: PORTD |= _BV(PB6); break;
	case 2: PORTD |= _BV(PB7); break;
	case 3: PORTB |= _BV(PB0); break;
	case 4: PORTB |= _BV(PB3); break;
	case 5: PORTB |= _BV(PB4); break;
#endif

	}

}

/*
 * Set blinking on or off
 * \param display: Display number 0 or 1
 * \param blink: 1 for blinking mode on
 *
 */
void LED_SetBlinking(unsigned char display, unsigned char blink) {
	digit_blink[display] = blink;
}

/*
 * Set a number from 0..999 to the display
 *
 * \param display: Display number 0 or 1
 * \param number: an integer from 0 to 999
 *
 */
void LED_SetNumber(unsigned char display, unsigned int number) {
	unsigned char idigit;
	int i;
	// Select the digit
	idigit = display * 3;

	// Set all 3 digits
	for (i = 0; i<3; i++) {
		digit[idigit+2-i] = '0' + (number % 10);
		number /= 10;
	}
}


/*
 * Diplay a text on the display
 *
 * \param display: Display number 0 or 1
 * \param text: a 3 character string
 *
 */
void LED_SetText(unsigned char display, char* text) {
	unsigned char idigit;
	int i;
	// Select the digit
	idigit = display * 3;

	// Set all 3 digits
	for (i = 0; i<3; i++) {
		digit[idigit+i] = text[i];
	}
}

/*
 * LED Task
 *
 */
void LED_Task(void) {
	unsigned char displayNr;

	// 1st or 2nd display ?
	if (led_mux >= 3 )
		displayNr = 1;
	else
		displayNr = 0;

	LED_DigitsOff();
	LED_SegmentsOff();

	if (!(digit_blink[displayNr] && blink)) {
		LED_SegmentsOn(digit[led_mux]);
		LED_DigitOn(led_mux);
	}

	// Next digit
	led_mux ++;
	if (led_mux >=6) {
		led_mux = 0;

		// Generate the blinking effect
		blink_cnt ++;
		if (blink_cnt > 10) {
			blink_cnt = 0;
			blink = !blink;
		}
	}

}

