/*
 * Timer0.c
 *
 * Created: 05.10.2013 22:38:59
 *  Author: Слава
 */ 
#include "Timer1.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Params.h"
#include "ADC.h"
#include "GlobalConstants.h"
#include "Flow.h"
#include "Fan.h"
#include <stdint.h>
#include "measQueue.h"
#include "Output.h"

uint64_t loops;	//количество тактов в интервале	
volatile uint32_t overflows;	//необходимое количество переполнений таймера
volatile uint8_t queueMeasPrecounter = 0; //Предделитель 
volatile static uint8_t beepCounter = 0;

void Timer1_Tick();	
void StateMachine();
uint8_t PatientDamage();
uint8_t PatientAlarm();
uint8_t PatientDiagnosticEnd();

ISR (TIMER1_OVF_vect)
{
	overflows--;
	if (overflows<=0)
	{
		TIMSK &= ~(1<<TOIE1);
		TIMSK |= 1<<OCIE1A;
	}
}

ISR (TIMER1_COMPA_vect, ISR_NOBLOCK)
{	
	TIMSK &= ~(1<<OCIE1A);
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
	PORTD &= ~(1<<PORTD6);
	*/
	if (alarmMuteTimer > 0)
	{
		alarmMuteTimer--;
	}
	
	fanTimer++;
	if (fanTimer > (uint16_t)savedParameters[FAN_PID_T].value) //10sec
	{
		fanTimer = 0;
		preMeasP_Fan = P_Fan;
		P_Fan = nonsavedParameters[O2_SET].value - Measurements[O2].value;
		D_Fan = P_Fan - preMeasP_Fan;
		fanSpeed = FanSpeedCalc(P_Fan, D_Fan);
		fanTime = FanTimeCalc(P_Fan, D_Fan);
	} 
	
	if (fanTime > fanTimer)
	{
		OCR2 = fanSpeed;
	}
	else
	{
		OCR2 = 0;
	}	
	
	if (Measurements[STATE].array[0] == DEVICE_PROCEDURE_DAMAGE_STATE || Measurements[STATE].array[0] == DEVICE_DIAGNOSTIC_END_STATE)
	{
		OCR2 = 250;
	}
	
	breathDirectionPre = breathDirection;
	if (fabs(Measurements[FlowT].value) > savedParameters[MINBR_FT].value)	//определяем вдох или выдох
	{
		if (Measurements[FlowT].value > 0.0)
		{
			breathDirection = 1;	//вдох
		} 
		else
		{
			breathDirection = -1;	//выдох
		}
	}
	else
	{
		breathDirection = 0;	//нет дыхания
	}
	
	if ((breathDirection == breathDirectionPre))// && breathDirection)	//Продолжается вдох/выдох
	{
		flowIntSum += ((Qprev1 + Measurements[FlowT].value) / 2.0) * (float)H_Step;
		breathTimer++;
		
		/*
		if ((breathDirection == 1) && (fanTimer > 0))
		{
			OCR2 = fanSpeed;
			fanTimer--;
		}
		else
		{
			OCR2 = 0;
		}
		*/		
	} 
	else if (breathDirectionPre != 0) //вдох/выдох закончился
	{
		if (breathDirectionPre > 0)	//закончился вдох
		{
			if ((flowIntSum > savedParameters[BR_V_MIN].value) && (flowIntSum < savedParameters[BR_V_MAX].value))
			{
				Measurements[Vin].value = flowIntSum;
			}
		} 
		else	//закончился выдох
		{
			if ((-flowIntSum > savedParameters[BR_V_MIN].value) && (-flowIntSum < savedParameters[BR_V_MAX].value))
			{
				Measurements[Vout].value = flowIntSum;
			
			
				if ((breathTimer > savedParameters[BR_T_MIN].value) && (breathTimer < savedParameters[BR_T_MAX].value)) //по оканчании вдоха, измеряем период дыхания
				{				
					Measurements[Fbreth].value = 60.0 / ((float)breathTimer / 100.0);
				}
				breathTimer = 0;
			}
			/*Вначале вдоха расччитываем параметры вентилятора*/
			/*
			P_Fan = nonsavedParameters[O2_SET].value - Measurements[O2].value;
			D_Fan = preMeasP_Fan - P_Fan;
			fanSpeed = FanSpeedCalc(P_Fan, D_Fan);
			fanTimer = FanTimeCalc(P_Fan, D_Fan);
			*/
		}
		flowIntSum = 0.0;
	}
	//OCR2 = savedParameters[IT_FAN].value;			
	
	queueMeasPrecounter++;
	if (queueMeasPrecounter >= 100)
	{
		DoQueueMeas();
		queueMeasPrecounter = 0;
	}
	
	if (queueMeasPrecounter<50 && beepCounter > 0)
	{
		Sound_On();
		beepCounter--;
	} 
	else
	{
		Sound_Off();
	}
	
	StateMachine();
}

void StateMachine()
{
	switch (Measurements[STATE].array[0])
	{
		case DEVICE_DIAGNOSTIC_STATE:
		{
			if (PatientDamage())
			{
				Measurements[STATE].array[0] = DEVICE_PROCEDURE_DAMAGE_STATE;
				break;
			}
			
			if (PatientAlarm())
			{
				Measurements[STATE].array[0] = DEVICE_PROCEDURE_ALARM_STATE;
				break;
			}
			
			if (PatientDiagnosticEnd())
			{
				Measurements[STATE].array[0] = DEVICE_DIAGNOSTIC_END_STATE;
				break;
			}
			break;
		}
	
	
		case DEVICE_THERAPY_STATE:
		{
			if (PatientDamage())
			{
				Measurements[STATE].array[0] = DEVICE_PROCEDURE_DAMAGE_STATE;
				break;
			}
			
			if (PatientAlarm())
			{
				Measurements[STATE].array[0] = DEVICE_PROCEDURE_ALARM_STATE;
				break;
			}
			break;
		}
		
		case DEVICE_PROCEDURE_ALARM_STATE:
		{
			beepCounter = 5;
			break;
		}
		
		case DEVICE_PROCEDURE_DAMAGE_STATE:
		{
			beepCounter = 255;
			break;
		}
		
		default:
		{
			beepCounter = 0;
			break;
		}
	}
}

uint8_t PatientDamage()
{
	return 0; ////УБРАТЬ!!!!!!!!!!!!!!!
	if (nonsavedParameters[MUTE].array[0] == 1 || alarmMuteTimer)
	return 0;
	
	if (Measurements[CO2].value > 1.0)
	{
		Measurements[DAMAGE].value = 1;
		return 1;
	}
	
	if ((Measurements[SPO2_AVG].value < 75) && Measurements[SPO2_DEV].value < 1.0)
	{
		Measurements[DAMAGE].value = 2;
		return 2;
	}
	
	if ((Measurements[HR_AVG].value < 40 || Measurements[HR_AVG].value > 140) && Measurements[HR_DEV].value < 1.0)
	{
		Measurements[DAMAGE].value = 3;
		return 3;
	}
	
	return 0;
}

uint8_t PatientAlarm()
{
	if (nonsavedParameters[MUTE].array[0] == 1 || alarmMuteTimer)
	return 0;
	
	if (Measurements[CO2].value > 0.8)
	{
		Measurements[ALARM].value = 1;
		return 1;
	}
	
	if ((Measurements[SPO2_AVG].value < 80.1) && Measurements[SPO2_DEV].value < 1.0)
	{
		Measurements[ALARM].value = 2;
		return 2;
	}
	
	if ((Measurements[HR_AVG].value < 50 || Measurements[HR_AVG].value > 110) && Measurements[HR_DEV].value < 1.0)
	{
		Measurements[ALARM].value = 3;
		return 3;
	}
	
	return 0;
}

uint8_t PatientDiagnosticEnd()
{
	if (Measurements[O2].value < 10.1)
	{
		Measurements[DIAG].value = 1;
		return 1;
	}
	
	if ((Measurements[SPO2_AVG].value < 80.1) && Measurements[SPO2_DEV].value < 1.0)
	{
		Measurements[DIAG].value = 2;
		return 2;
	}
	
	if ((Measurements[HR_AVG].value < 50 || Measurements[HR_AVG].value > 110) && Measurements[HR_DEV].value < 1.0)
	{
		Measurements[DIAG].value = 3;
		return 3;
	}	
	
	if (Measurements[Fbreth].value - Measurements[F_BR_START].value > Measurements[F_BR_START].value * 0.5)
	{
		Measurements[DIAG].value = 4;
		return 4;
	}
	
	if (Measurements[HR_AVG].value - Measurements[HR_START].value > Measurements[HR_START].value * 0.5)
	{
		Measurements[DIAG].value = 5;
		return 5;
	}
	
	return 0;
}