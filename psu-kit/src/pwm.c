/*
 *  Project:      psu-kit
 *  File:         pwm.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  PWM generation for charge pump and boost converter
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
#include "pwm.h"

/*
 * Initialize the PWM module
 */
void PWM_Init(void) {

	// OC1A and OC1B as output
	DDRB |= _BV(PB1);
	DDRB |= _BV(PB2);
	PORTB &= ~_BV(PB1);
	PORTB &= ~_BV(PB2);

	// Set at BOTTOM, Clear at match
	TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11);

	// Fast PWM, Top of ICR1, prescaler: 1:1
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);

	// Set period time to 62,5kHz
	ICR1 = BUCK_PWM_MAX;

	// Set PWM of boost to 0%
	OCR1A = 0x0080;

	// Set PWM of buck to 0%
	OCR1B = 0x0000;
}

/*
 * Set the PWM to the OCR1B output (buck-converter)
 *
 * \param val PWM value from 0 to 128=100%
 */
void PWM_SetBuckPWM(unsigned char val) {

	OCR1B = val;
}
/*
 * Set the PWM to the OCR1A output (boost-converter)
 *
 * \param val PWM value from 0 to 64(BOOST_PWM_MAX)=50%
 */
void PWM_SetBoostPWM(unsigned char val) {

	if (val > BOOST_PWM_MAX) { // max +50%
		val = BOOST_PWM_MAX;
	}
	OCR1A = 128-val;
}
