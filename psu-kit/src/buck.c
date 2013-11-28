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
 * defines
 */

//Parameter buck controller
#define BUCK_KP 8
#define BUCK_KI 2

/*
 * local variables
 */
static unsigned int volt_drop;
static signed int buck_pwm;
static signed int buck_pwm_int = 0;
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
	unsigned int buck_sp;
	signed int regdiff;

	// Calculate the setpoint
	buck_sp = ADC_GetScaled(ADC_CHAN_V_SET) + volt_drop;

/*
	// Test the controller
	if (show_setpoints)
		buck_sp  = 180;
	else
		buck_sp = 80;
*/

	// Calculate the difference
	regdiff = buck_sp - ADC_GetScaled(ADC_CHAN_V_BUCK);

	// Integrate
	buck_pwm_int += BUCK_KI * regdiff;

	// Calculate the PWM value
	buck_pwm = BUCK_KP * regdiff / 16 + buck_pwm_int / 256;

	// Limit the PWM to min and max and limit also the integrator
	if (buck_pwm > BUCK_PWM_MAX) {
		buck_pwm = BUCK_PWM_MAX;
		buck_pwm_int = ( BUCK_PWM_MAX - (BUCK_KP * regdiff / 16) ) * 256;
	}
	if (buck_pwm < 0) {
		buck_pwm = 0;
		buck_pwm_int = ( 0 - (BUCK_KP * regdiff / 16) ) * 256;
	}

	// 100% if voltage drop feature is switched off
	if (voltage_drop == VOLTDROP_OFF)
		buck_pwm = BUCK_PWM_MAX;

	// 100% if voltage drop "auto" is selected
	if (voltage_drop == VOLTDROP_AUTO)
		buck_pwm = BUCK_PWM_MAX;

	// Set the PWM
	PWM_SetBuckPWM(buck_pwm);

}
