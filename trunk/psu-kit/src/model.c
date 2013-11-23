/*
 *  Project:      psu-kit
 *  File:         model.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  holds global values
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

unsigned char setup_active = 0;
unsigned char show_setpoints = 0;
unsigned char appl_thermo = 0;
unsigned char thermo_model = 25;
unsigned char startup = 1;
unsigned int startup_cnt = 0;
t_SetupParam setup_param = SP_DROP;
unsigned char voltage_drop = 5;
unsigned char rth = 3;
unsigned char tth = 8;

