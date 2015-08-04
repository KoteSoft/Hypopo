/*
 * Output.c
 *
 * Created: 20.10.2013 16:00:21
 *  Author: Слава
 */ 

#include "avr/io.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

volatile static uint8_t sound_on_flag;

void Sound_On()
{
	sound_on_flag = 1;
}

void Sound_Off()
{	
	sound_on_flag = 0;
}

ISR(TIMER2_OVF_vect)
{
	if (sound_on_flag)
	{
		if (bit_is_set(PORTD, PORTD6))
			{
				PORTD &= ~(1 << PORTD6);	
			} 
			else
			{
				PORTD |= 1<<PORTD6;
			}
	} 
	else
	{
		PORTD |= 1<<PORTD6;
	}
}

void SoundStartMusic()
{
	for (int32_t i = 0; i < 50; i++)
	{
		if (bit_is_set(PORTD, PORTD6))
		{
			PORTD &= ~(1 << PORTD6);
		}
		else
		{
			PORTD |= 1<<PORTD6;
		}
		_delay_us(700);
	}
	
	_delay_ms(50);
	
	for (int32_t i = 0; i < 50; i++)
	{
		if (bit_is_set(PORTD, PORTD6))
		{
			PORTD &= ~(1 << PORTD6);
		}
		else
		{
			PORTD |= 1<<PORTD6;
		}
		_delay_us(700);
	}
	
	PORTD &= ~(1 << PORTD6);
}