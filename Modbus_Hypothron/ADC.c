/*
 * ADC.c
 *
 * Created: 08.10.2013 0:07:40
 *  Author: Слава
 */ 
#include <avr/io.h>
#include "ADC.h"

ISR(ADC_vect)
{
	ADC_Result[ADMUX]=ADC;//ADCH<<8|ADCL;
	switch (ADMUX)
	{
		case 0:
			ADMUX = 1;
		break;
		
		case 1:
			ADMUX = 2;
		break;
		
		default:
			ADMUX = 0;
	}
	/*
	if (ADMUX==0)
	{
		ADMUX=1;
	}
	else if (ADMUX==1)
	{
		ADMUX=2;
	}
	else
	{
		ADMUX=0;
	}
	*/
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
}

void ADC_Init()
{
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
	SFIOR = 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0;
	ADMUX=0;
}