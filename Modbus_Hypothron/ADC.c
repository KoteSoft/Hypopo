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

volatile uint16_t ADC_Result[8];

/*Не работает*/
ISR(ADC_vect)
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

void ADC_Poll()
{
	Measurements[ADC0].value = (ADC_Result[0] * U_ref) / 1023.0 + savedParameters[OFF_F1].value;
	Measurements[ADC1].value = (ADC_Result[1] * U_ref) / 1023.0 + savedParameters[OFF_F2].value;
	Measurements[ADC2].value = (ADC_Result[2] * U_ref) / 1023.0;	
	Measurements[ADC3].value = (ADC_Result[3] * U_ref) / 1023.0;
	Measurements[ADC4].value = (ADC_Result[4] * U_ref) / 1023.0;
	Measurements[ADC5].value = (ADC_Result[5] * U_ref) / 1023.0;
	Measurements[ADC6].value = (ADC_Result[6] * U_ref) / 1023.0;
	Measurements[ADC7].value = (ADC_Result[7] * U_ref) / 1023.0;
}