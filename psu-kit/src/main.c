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


/*
 * Main function
 */
int main( void )
{
	volatile unsigned int i;

	//Initialize all modules
	PWM_Init();
	LED_Init();
	ADC_Init();

	// Set the converter voltage
	PWM_SetBuckPWM(64);


	LED_SetNumber(0, 543);
	LED_SetText(1, "ABC");

	while(1) {

		ADC_Task();
		LED_Task();

		for (i=0; i<1000;i++) {
			;
		}
		LED_SetNumber(0, ADC_GetScaled(ADC_CHAN_V_OUT));
		LED_SetNumber(1, ADC_GetScaled(ADC_CHAN_I_OUT));
	}

}
