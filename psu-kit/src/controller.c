/*
 *  Project:      psu-kit
 *  File:         controller.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  Controls the 2 keys for setup and other stuff
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
#include "controller.h"
#include "model.h"
#include "key.h"

/*
 * Initialize the controller
 */
void CONTROLLER_Init(void) {

}

/*
 * Controller task
 *
 */
void CONTROLLER_Task(void) {

	// Startup
	if (startup) {
		startup_cnt++;
		if (startup_cnt > 1000) {
			if (KEY_Pressed(SW1)) {
				setup_active = 1;
				KEY_Reset();
			}
			startup = 0;
		}

	}

	// Setup Mode
	if (setup_active) {

		// Next Setup Menu
		if (KEY_Click(SW1)) {
			setup_param ++;

			// Exit setup mode
			if (setup_param == SP_LAST) {
				setup_active = 0;
			}

		}

		// Increment the parameter
		if (KEY_Click(SW2)) {

			switch (setup_param) {

			// Voltage drop
			case SP_DROP:
				voltage_drop +=1;
				if (voltage_drop > VOLTDROP_AUTO) {
					voltage_drop = 0;
				}
				break;

			// Thermal resistance
			case SP_RTH:
				rth +=1;
				if (rth > 50) {
					rth = 1;
				}
				break;

			// Thermal time constant
			case SP_TTH:
				tth +=2;
				if (tth > 60) {
					tth = 2;
				}
				break;

			// Applicate the thermo model
			case SP_APPL_THERMO:
				appl_thermo ++;
				if (appl_thermo > 1) {
					appl_thermo = 0;
				}
				break;

			// Thermal time constant
			default:

				break;
			}


		}

	}

}
