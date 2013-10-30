/*
 * Output.c
 *
 * Created: 20.10.2013 16:00:21
 *  Author: Слава
 */ 

#include "avr/io.h"

void Sound_On()
{
	PORTD |= 1<<PORTD6;
}

void Sound_Off()
{	
	PORTD &= ~(1 << PORTD6);	
}