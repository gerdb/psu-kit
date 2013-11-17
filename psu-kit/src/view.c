/*
 *  Project:      psu-kit
 *  File:         view.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  View for different display types
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
#include "view.h"
#include "led.h"
#include "adc.h"
#include "model.h"

/*
 * local variables
 */
static unsigned char blink = 0;
static unsigned int blink_cnt = 0;

/*
 * Initialize the view
 */
void VIEW_Init(void) {

}

/*
 * View task
 *
 */
void VIEW_Task(void) {

	if (startup) {
		LED_SetText(0, "PSU");
		LED_SetText(1, "001");
	} else if (setup_active) {
		// Setup menu
		switch (setup_param) {

		// Voltage drop
		case SP_DROP:
			LED_SetText(0, "DRP");
			if (voltage_drop == VOLTDROP_OFF)
				LED_SetText(1, "OFF");
			else if (voltage_drop == VOLTDROP_AUTO)
				LED_SetText(1, "AUT");
			else
				LED_SetNumber(1, voltage_drop, 1);
			break;

		// Thermal resistance
		case SP_RTH:
			LED_SetText(0, "RTH");
			LED_SetNumber(1, rth, 1);
			break;

		// Thermal time constant
		case SP_TTH:
			LED_SetText(0, "TTH");
			LED_SetNumber(1, tth, 1);
			break;

		// Applicate the thermo model
		case SP_APPL_THERMO:
			LED_SetText(0, "APL");
			LED_SetNumber(1, appl_thermo, 1);
			break;

		// Thermal time constant
		default:
			LED_SetText(0, "---");
			LED_SetText(1, "---");
			break;
		}


	} else {

		if (appl_thermo && blink ) {
			// Display the thermo model temperature
			LED_SetNumber(0, thermo_model, 1);

		} else {
			// Display the output voltage
			LED_SetNumber(0, ADC_GetScaled(ADC_CHAN_V_OUT), 0);
		}

		// Display the output current
		LED_SetNumber(1, ADC_GetScaled(ADC_CHAN_I_OUT), 0);
	}

	// Generate the blinking effect
	blink_cnt++;
	if (blink_cnt > 400) {
		blink_cnt = 0;
		blink = !blink;
	}
}
