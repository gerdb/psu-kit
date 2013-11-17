/*
 *  Project:      psu-kit
 *  File:         buck.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  controls the buck converter
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
#include "adc.h"
#include "model.h"
#include "buck.h"

/*
 * local variables
 */
static unsigned int volt_drop;
static unsigned char buck_pwm;
static unsigned char buck_pwm_int = 0;
/*
 * Initialize the buck controller
 */
void BUCK_Init(void) {
	PWM_SetBuckPWM(0);

	// Scale it to 0.1V
	volt_drop = voltage_drop * 10;
}

/*
 * Buck task
 *
 */
void BUCK_Task(void) {
	unsigned int buck_out;

	// Calculate the setpoint
	buck_out = ADC_GetScaled(ADC_CHAN_V_SET) + volt_drop;

	// Calculate the PWM value
	if (ADC_GetScaled(ADC_CHAN_V_IN) > buck_out) {
		buck_pwm = BUCK_PWM_MAX * buck_out / ADC_GetScaled(ADC_CHAN_V_IN);
	} else {
		buck_pwm = BUCK_PWM_MAX;
	}

	// 100% if voltage drop feature is switched off
	if (voltage_drop == VOLTDROP_OFF)
		buck_pwm = BUCK_PWM_MAX;

	// 100% if voltage drop "auto" is selected
	if (voltage_drop == VOLTDROP_AUTO)
		buck_pwm = BUCK_PWM_MAX;


	// Increment or decrement the pwm slowly
	if (buck_pwm > buck_pwm_int)
		buck_pwm_int ++;
	if (buck_pwm < buck_pwm_int)
		buck_pwm_int --;

	// Set the PWM
	PWM_SetBuckPWM(buck_pwm_int);

}
