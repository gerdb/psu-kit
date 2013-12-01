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

#define BOOST_KP 2
#define BOOST_KI 1

/*
 * local variables
 */
static unsigned int volt_drop;
static signed int out_pwm;
static signed int buck_pwm_int = 0;
static signed int old_v_buck = 0 ;
static signed int v_buck = 0 ;
static unsigned char buckboost_mode = BUCK_MODE;
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

	// Limit setpoint to 36.0V
	if (buck_sp > 360)
		buck_sp = 360;

	// Get the output voltage of the buck converter
	v_buck = ADC_GetScaled(ADC_CHAN_V_BUCK);

	// Calculate the difference
	regdiff = buck_sp - v_buck;

	// Integrate
	if (buckboost_mode == BUCK_MODE)
		buck_pwm_int += BUCK_KI * regdiff;
	else
		buck_pwm_int += BOOST_KI * regdiff;


	// Limit
	if (buck_pwm_int > (3* BUCK_PWM_MAX * 16))
		buck_pwm_int = (3* BUCK_PWM_MAX * 16);

	if (buck_pwm_int < -(BUCK_PWM_MAX * 16))
		buck_pwm_int = -(BUCK_PWM_MAX * 16);


	// Calculate the PWM value
	if (buckboost_mode == BUCK_MODE) {
		out_pwm =  - BUCK_KP * v_buck / 16
					+ buck_pwm_int / 16
					- BUCK_KD * ( v_buck - old_v_buck ) / 16;
	} else {
		out_pwm =  - BOOST_KP * v_buck / 16
					+ buck_pwm_int / 16;
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
		buckboost_mode = BUCK_MODE;
	} else {
		PWM_SetBuckPWM(BUCK_PWM_MAX);

		// Fast overvoltage shutdown of boost
		if (v_buck > 400)
			PWM_SetBoostPWM(0);
		else
			PWM_SetBoostPWM(out_pwm-BUCK_PWM_MAX);

		buckboost_mode = BOOST_MODE;
	}


}
