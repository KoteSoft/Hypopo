/*
* Timer0.c
*
* Created: 05.10.2013 22:38:59
*  Author: Слава
*/
#include <Timer1.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Params.h>
#include <ADC.h>
#include <GlobalConstants.h>
#include <Flow.h>

uint64_t loops;	//количество тактов в интервале
volatile uint32_t overflows;	//необходимое количество переполнений таймера

void Timer1_Tick();

ISR (TIMER1_OVF_vect)
{
    overflows--;
    if (overflows<=0)
    {
        TIMSK &= ~1<<TOIE1;
        TIMSK |= 1<<OCIE1A;
    }
}

ISR (TIMER1_COMPA_vect)
{
    TIMSK &= ~1<<OCIE1A;
    TIMSK |= 1<<TOIE1;
    overflows = loops / 65536;
    
    Timer1_Tick();
}

uint8_t Timer1_Init(uint32_t interval)
{
    loops = ((uint64_t)interval * F_CPU) / 1000;
    overflows = loops / 65536;
    OCR1A = loops % 65536;
    TCCR1A = 0<<COM1A1 | 0<<COM1A0 | 0<<COM1B1 | 0<<COM1B0 | 0<<FOC1A | 0<<FOC1B | 0<<WGM11 | 0<<WGM10;
    TCCR1B = 0<<ICNC1 | 0<<ICES1 | 0<<WGM13 | 0<<WGM12 | 0<<CS12 | 0<<CS11 | 1<<CS10;
    TIMSK |= 1<<TOIE1;
    
    return 0;
}

void Timer1_Tick()
{
    /*
    PORTD |= 1<<PORTD6;
    _delay_ms(10);
    PORTD &= ~1<<PORTD6;
    */
    Measurements[ADC0].value = (ADC_Result[0] * U_ref) / 1023.0;
    Measurements[ADC1].value = (ADC_Result[1] * U_ref) / 1023.0;
    Measurements[ADC2].value = (ADC_Result[2] * U_ref) / 1023.0;
    
}