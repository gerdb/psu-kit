/*
 *  Project:      psu-kit
 *  File:         adc.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  Analog digital conversion of voltage and current
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
#include "adc.h"

/*
 * local functions
 */

/*
 * local variables
 */
unsigned int adc_raw[5] = {0,0,0,0,0};
unsigned int adc_scaled[5] = {0,0,0,0,0};
unsigned char adc_mux = 0;
unsigned char adc_filtcnt = 0;
unsigned int adc_filt = 0;

/*
 * Initialize the ADC module
 */
void ADC_Init(void) {

	// Ext Vref
	ADMUX = 0x00;

	//ADC on
	// 1:64 prescaler = 125kHz
	ADCSRA = 0x86;
	ADC_Start(0);
}

/*
 * Start an ADC conversion
 *
 * \param channel: The ADC channel
 */
void ADC_Task(void) {

	// Wait until the ADC result is available
	if ((ADCSRA & (1<<ADSC)) == 0) {

		// Get the ADC result and filter it
		adc_filt += ADCW;
		adc_filtcnt ++;

		// n values sampled ?
		if (adc_filtcnt >= 16) {
			adc_filtcnt = 0;
			adc_raw[adc_mux] = adc_filt;
			ADC_Scale(adc_mux);

			adc_filt = 0;

			// Next channel
			adc_mux ++;
			if (adc_mux >= 5) {
				adc_mux = 0;
			}

		}

		// Start the next conversion
		ADC_Start(adc_mux);

	}


}

/*
 * Start an ADC conversion
 *
 * \param channel: The ADC channel
 */
void ADC_Start(unsigned char channel) {

	// Select the channel
	// left justified result
	ADMUX = 0x00 | channel;

	// Start the conversion
	ADCSRA |= (1<<ADSC);
}

/*
 * Gets the raw value of an ADC channel
 *
 * \return : The ADC result
 */
unsigned int ADC_GetRaw(unsigned char channel) {

	// return the result
	return adc_raw[channel];
}

/*
 * Gets the scaled value of an ADC channel
 *
 * \return : The ADC result
 */
unsigned int ADC_GetScaled(unsigned char channel) {

	// return the result
	return adc_scaled[channel];
}

/*
 * Scale the ADC result
 *
 * \param channel : Channel to scale
 */
void ADC_Scale(unsigned char channel) {

	unsigned int raw = adc_raw[channel] / 16;

	switch (channel) {

	// Voltage divider: 6:1
	// 1024 = 5.0V = 30.0V
	// 1024 / 2 * 75 / 128 = 300
	case ADC_CHAN_V_SET:
	case ADC_CHAN_V_OUT: adc_scaled[channel] = (raw / 2) * 75 / 128;
	break;

	// Shunt 0.1R, gain: 21 = 2,1R
	// 1024 = 5.0V = 2.38095A
	// 1024 * 30 / 128 - 1024 / 512 = 238 ( instead of 238,985 )
	case ADC_CHAN_I_SET:
	case ADC_CHAN_I_OUT: adc_scaled[channel] = raw * 30 / 128 - raw / 512;
	break;

	// Voltage divider: 8.5:1
	// 1024 = 5.0V = 42.5V
	// 1024 * 53 / 128 = 424 (instead of 425)
	case ADC_CHAN_V_IN: adc_scaled[channel] = raw  * 53 / 128;
	break;
	}

}
