/*
 *  Project:      psu-kit
 *  File:         model.h
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  header file for model.c
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

#ifndef MODEL_H_
#define MODEL_H_

/*
 * Typedefs
 */

typedef enum {
	SP_DROP,
	SP_RTH,
	SP_TTH,
	SP_APPL_THERMO,
	SP_LAST
} t_SetupParam;

#define VOLTDROP_OFF 11
#define VOLTDROP_AUTO 12

/*
 * global variables
 */
extern unsigned char setup_active;
extern unsigned char show_setpoints;
extern unsigned char appl_thermo;
extern unsigned char thermo_model;
extern unsigned char startup;
extern unsigned int startup_cnt;
extern t_SetupParam setup_param;
extern unsigned char voltage_drop;
extern unsigned char rth;
extern unsigned char tth;
#endif /* MODEL_H_ */
