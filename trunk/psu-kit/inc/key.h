/*
 *  Project:      psu-kit
 *  File:         key.h
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  header file for key.c
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

#ifndef KEY_H_
#define KEY_H_

// Defines
#define SW1 0
#define SW2 1
#define DEBOUNCE 20

// Function prototypes
void KEY_Init(void);
void KEY_Task(void);
unsigned char KEY_Pressed(unsigned char key);
unsigned char KEY_Click(unsigned char key);


#endif /* KEY_H_ */
