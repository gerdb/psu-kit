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
 * Initialize the view
 */
void VIEW_Init(void) {

}

/*
 * View task
 *
 */
void VIEW_Task(void) {

	if (setup_active) {
		// Setup menu
		switch (setup_param) {
		case SP_DROP:
			LED_SetText(0, "DRP");
			LED_SetNumber(1, voltage_drop, 1);
			break;
		}

	} else {

		// Display the output voltage and current
		LED_SetNumber(0, ADC_GetScaled(ADC_CHAN_V_OUT), 0);
		LED_SetNumber(1, ADC_GetScaled(ADC_CHAN_I_OUT), 0);
	}
}
