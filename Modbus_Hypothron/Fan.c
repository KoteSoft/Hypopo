/*
 * Fan.c
 *
 * Created: 24.11.2013 20:59:20
 *  Author: Слава
 */ 
#include "Fan.h"
#include "Flow.h"
#include "avr/io.h"
#include "Params.h"
#include "avr/interrupt.h"

float P_Fan;
float D_Fan;
float preMeasP_Fan;
uint16_t fanTimer;
uint8_t fanSpeed;

void PWM_Timer2_Init()
{
	TCCR2 = 0<<FOC2 | 1<<WGM20 | 1<<COM21 | 0<<COM20 | 1<<WGM21 | 0<<CS22 | 0<<CS21 | 1<<CS20;
	OCR2 = 0;
}

uint8_t FanSpeedCalc(float P, float D)
{
	float Out = 0.0;
	
	Out = P * savedParameters[PI_FAN].value;
	Out += D * savedParameters[DI_FAN].value;
		
	if (Out > 255.0)
	{
		Out = 255.0;
	}
	else if (Out < savedParameters[FAN_MIN].value)
	{
		Out = 0.0;
	}
	
	return (uint8_t)Out;
}

uint16_t FanTimeCalc(float P, float D)
{
	float Out = 0.0;
		
	Out = P * savedParameters[PT_FAN].value;
	Out += D * savedParameters[DT_FAN].value;
		
	if (Out > 65500.0)
	{
		Out = 65500.0;
	}
	else if (Out < 0.0)
	{
		Out = 0.0;
	}
		
	return (uint16_t)Out;
}