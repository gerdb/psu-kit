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


/*
 * Initialize the ADC module
 */
void ADC_Init(void) {

	// Ext Vref
	// ADC: int 1.3V ref
	ADMUX = 0x0E;

	//ADC on
	// 1:64 prescaler = 125kHz
	ADCSRA = 0x86;
}



/*
 * Start an ADC conversion
 *
 * \param channel: The ADC channel
 */
void ADC_Start(unsigned char channel) {

	// Select the channel
	// Right justified result
	ADMUX = 0x20 | channel;

	// Start the conversion
	ADCSRA |= (1<<ADSC);
}

/*
 * Returns the ADC result. Wait until the ADC is ready
 *
 * \return : The ADC result
 */
unsigned int ADC_GetResult(void) {

	// wait for conversion to complete
	while(ADCSRA & (1<<ADSC));

	// return the result
	return ADCH;
}



