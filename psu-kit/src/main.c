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

/*
 * Main function
 */
int main(void) {
	volatile unsigned int i;

	//Initialize all modules
	PWM_Init();
	LED_Init();
	ADC_Init();
	KEY_Init();
	CONTROLLER_Init();
	VIEW_Init();

	// Set the converter voltage
	PWM_SetBuckPWM(64);

	while (1) {

		ADC_Task();
		LED_Task();
		KEY_Task();
		CONTROLLER_Task();
		VIEW_Task();

		for (i = 0; i < 1000; i++) {
			;
		}

	}

}
