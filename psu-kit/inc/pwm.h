/*
 *  Project:      psu-kit
 *  File:         pwm.h
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  header file for pwm.c
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

#ifndef PWM_H_
#define PWM_H_

#define BUCK_PWM_MAX 128
#define BOOST_PWM_MAX 48

//Function prototypes
void PWM_Init(void);
void PWM_SetBuckPWM(unsigned char val);
void PWM_SetBoostPWM(unsigned char val);

#endif /* PWM_H_ */
