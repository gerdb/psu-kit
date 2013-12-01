/*
 *  Project:      psu-kit
 *  File:         buckboost.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  controls the buck and boost converter
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
#include "buckboost.h"


/*
 * defines
 */

//Parameter buck controller
#define BUCK_KD 0
#define BUCK_KP 8
#define BUCK_KI 2

/*
 * local variables
 */
static unsigned int volt_drop;
static signed int out_pwm;
static signed int buck_pwm_int = 0;
static signed int old_v_buck = 0 ;
static signed int v_buck = 0 ;
/*
 * Initialize the buck controller
 */
void BUCKB_Init(void) {
	PWM_SetBuckPWM(0);

	// Scale it to 0.1V
	volt_drop = voltage_drop * 10;
}

/*
 * Buck task
 *
 */
void BUCKB_Task(void) {
	unsigned int buck_sp;
	signed int regdiff;

	// Calculate the setpoint
	buck_sp = ADC_GetScaled(ADC_CHAN_V_OUT) + volt_drop;

	v_buck = ADC_GetScaled(ADC_CHAN_V_BUCK);
/*
	// Test the controller
	if (show_setpoints)
		buck_sp  = 180;
	else
		buck_sp = 80;
*/


	// Calculate the difference
	regdiff = buck_sp - v_buck;

	// Integrate
	buck_pwm_int += BUCK_KI * regdiff;
	// Limit
	if (buck_pwm_int > (3* BUCK_PWM_MAX * 16)) {
		buck_pwm_int = (3* BUCK_PWM_MAX * 16);
	}
	if (buck_pwm_int < -(BUCK_PWM_MAX * 16)) {
		buck_pwm_int = -(BUCK_PWM_MAX * 16);
	}

	// Calculate the PWM value
	out_pwm =  - BUCK_KP * v_buck / 16
				+ buck_pwm_int / 16
				- BUCK_KD * ( v_buck - old_v_buck ) / 16;

	// Limit the PWM to min and max and limit also the integrator
	if (out_pwm > (BUCK_PWM_MAX+BOOST_PWM_MAX)) {
		out_pwm = (BUCK_PWM_MAX+BOOST_PWM_MAX);
		buck_pwm_int = ( (BUCK_PWM_MAX+BOOST_PWM_MAX) + (BUCK_KP * v_buck / 16) ) * 16;
	}
	if (out_pwm < 0) {
		out_pwm = 0;
		buck_pwm_int = ( 0 + (BUCK_KP *  v_buck / 16) ) * 16;
	}

	// 100% if voltage drop feature is switched off
	if (voltage_drop == VOLTDROP_OFF)
		out_pwm = BUCK_PWM_MAX;

	// 100% if voltage drop "auto" is selected
	if (voltage_drop == VOLTDROP_AUTO)
		out_pwm = BUCK_PWM_MAX;

	// for the next task
	old_v_buck = v_buck;

	// Set the PWM
	if (out_pwm<= BUCK_PWM_MAX) {
		PWM_SetBuckPWM(out_pwm);
		PWM_SetBoostPWM(0);
	} else {
		PWM_SetBuckPWM(BUCK_PWM_MAX);
		PWM_SetBoostPWM(out_pwm-BUCK_PWM_MAX);
	}


}
