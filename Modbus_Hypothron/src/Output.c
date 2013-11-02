/*
* Output.c
*
* Created: 20.10.2013 16:00:21
*  Author: Слава
*/

#include <avr/io.h>

void SoundOn()
{
    PORTD |= 1<<PORTD6;
}

void SoundOff()
{
    PORTD &= ~(1 << PORTD6);
}