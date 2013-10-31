/*
 * ADC.c
 *
 * Created: 08.10.2013 0:07:40
 *  Author: Слава
 */ 
#include <avr/io.h>
#include "ADC.h"
#include "GlobalConstants.h"
#include "Params.h"

/*Не работает*/
ISR(ADC_vect, ISR_BLOCK)
{
	Measurements[ADC2].value = (ADC * U_ref) / 1023.0;
	//ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
}

void ADC_Init()
{
	ADCSRA=1<<ADEN|1<<ADSC|1<<ADATE|0<<ADIF|0<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
    SFIOR = 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0;
    ADMUX=2;
}

uint8_t ADC_Num = 0;
void Get_ADC()
{		
	ADC_Result[ADC_Num] = ADC;//(ADC * U_ref) / 1023.0;
	ADC_Num++;
	if (ADC_Num >= 8)
	{
		ADC_Num = 0;
	}
	ADMUX = ADC_Num;
}