/*
 *  Project:      psu-kit
 *  File:         pwm.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  main file
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
#include "led.h"
#include "adc.h"
#include "controller.h"
#include "model.h"
#include "view.h"
#include "key.h"
#include "buckboost.h"

void MAIN_Init(void) {
	// Timer setup
	// Set timer prescaler to 1:64
	// and so the timer frequency to 8MHz/64/256 = 2.048ms
	TCCR0 = 0b00000011;
}

/*
 * Main function
 */
int main(void) {

	//Initialize all modules
	PWM_Init();
	BUCKB_Init();
	LED_Init();
	ADC_Init();
	KEY_Init();
	CONTROLLER_Init();
	VIEW_Init();
	MAIN_Init();

	while (1) {

		// All tasks
		ADC_Task();
		BUCKB_Task();
		LED_Task(); // 6 digits in a 2ms task: 81Hz
		KEY_Task();
		CONTROLLER_Task();
		VIEW_Task();

		// 2.048ms Task
		while ((TIFR & _BV(TOV0)) == 0);
		// Clear the flag
		TIFR |= _BV(TOV0);
	}

}
