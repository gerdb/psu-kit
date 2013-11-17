/*
 *  Project:      psu-kit
 *  File:         key.c
 *  Author:       gerd bartelt - www.sebulli.com
 *
 *  Description:  reads and debounces the two keys
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
#include "key.h"

unsigned char debounce_cnt[2] = { 0, 0 };
unsigned char key_pressed[2] = { 0, 0 };
unsigned char key_click[2] = { 0, 0 };

/*
 * Initialize the key module
 */
void KEY_Init(void) {

	// Both Ports PB5 and PC5 as input
	DDRB &= ~_BV(DDB5);
	DDRC &= ~_BV(DDC5);

	// Enable PullUps
	PORTB |= _BV(PB5);
	PORTC |= _BV(PC5);

}

/*
 * Key task
 *
 */
void KEY_Task(void) {
	unsigned char key;
	unsigned char i;

	// Do it for both keys
	for (i = 0; i < 2; i++) {

		// Read the pin
		if (i == SW1)
			key = PINC & _BV(PINC5);
		if (i == SW2)
			key = PINB & _BV(PINB5);

		// Is input LOW = active ?
		if (key == 0) {

			// Count up the debounce counter
			if (debounce_cnt[i] < DEBOUNCE) {
				debounce_cnt[i]++;
			} else {
				// pressed for n tasks. It will now be marked as pressed
				// But first we detect a 0->1 edge as "click"
				if (!key_pressed[i])
					key_click[i] = 1;
				key_pressed[i] = 1;
			}
		} else {
			// Count down the debounce counter
			if (debounce_cnt[i] > 0) {
				debounce_cnt[i]--;
			} else {
				// released for n tasks. It will now be marked as released
				key_pressed[i] = 0;
			}
		}
	}

}

/*
 * Get the state of the key
 *
 * \param key: The key
 */
unsigned char KEY_Pressed(unsigned char key) {

	return key_pressed[key];

}

/*
 * Get a click event
 *
 * \param key: The key
 */
unsigned char KEY_Click(unsigned char key) {
	unsigned char ret;
	ret = key_click[key];

	// Reset the event
	key_click[key] = 0;

	return ret;

}

/*
 * Reset all key events
 *
 */
void KEY_Reset(void) {

	// Reset the events
	key_click[SW1] = 0;
	key_click[SW2] = 0;
}

