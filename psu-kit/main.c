/*
 * main.c
 *
 *  Created on: Nov 10, 2013
 *      Author: gerd
 */


#include <avr/io.h>

#define running  1


void delay_ms( unsigned int ms )
{
    unsigned int  i;
    unsigned int  j;

    for ( i = ms;  i;  i-- )
    {
        for ( j = 51;  j;  j-- )
        {
        }
    }
}

int main( void )
{
    // PortD6 as output
    DDRD |= _BV(PD6);

    // main loop
    while ( running )
    {
    	// LED on
        PORTD |= _BV(PD6);
        delay_ms( 1000 );

    	// LED off
        PORTD &= ~_BV(PD6);
        delay_ms( 29000 );
    }
}
