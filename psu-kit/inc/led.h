/*
 *  Project:      psu-kit
 *  File:         led.h
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  header file for led.c
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

#ifndef LED_H_
#define LED_H_


// Defines
/* 7-segment character bit assignments */
#define SEG_A     _BV(PB0)
#define SEG_B     _BV(PB1)
#define SEG_C     _BV(PB2)
#define SEG_D     _BV(PB3)
#define SEG_E     _BV(PB4)
#define SEG_F     _BV(PB6)
#define SEG_G     _BV(PB7)

//Function prototypes
void LED_Init(void);
void LED_Task(void);
void LED_SetBlinking(unsigned char display, unsigned char blink);
void LED_SetNumber(unsigned char display, unsigned int number);

#endif /* LED_H_ */
