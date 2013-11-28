/*
 *  Project:      psu-kit
 *  File:         adc.h
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  header file for adc.c
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

#ifndef ADC_H_
#define ADC_H_

// Defines
#define ADC_CHAN_V_SET 0
#define ADC_CHAN_I_SET 1
#define ADC_CHAN_V_OUT 2
#define ADC_CHAN_I_OUT 3
#define ADC_CHAN_V_BUCK 4

// Function prototypes
void ADC_Init(void);
void ADC_Start(unsigned char channel);
void ADC_Task(void);
unsigned int ADC_GetRaw(unsigned char channel);
unsigned int ADC_GetScaled(unsigned char channel);
void ADC_Scale(unsigned char channel);

#endif /* ADC_H_ */
