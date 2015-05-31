/*
 * measQueue.c
 *
 * Created: 07.02.2015 17:34:41
 *  Author: Слава
 */ 

#include "measQueue.h"
#include <math.h>
#include "Params.h"

uint8_t spo2Queue[SPO2_QUEUE_SIZE];
uint8_t hrQueue[HR_QUEUE_SIZE];

void Spo2Enqueue()
{
	for (uint8_t i = 1; i < SPO2_QUEUE_SIZE; i++)
	{
		spo2Queue[i] = spo2Queue[i - 1];
	}
	spo2Queue[0] = (uint8_t)(Measurements[SPO2].value);
}

void HrEnqueue()
{
	for (uint8_t i = 1; i < HR_QUEUE_SIZE; i++)
	{
		hrQueue[i] = hrQueue[i - 1];
	}
	hrQueue[0] = (uint8_t)(Measurements[HR].value);
}

float Spo2Avg()
{
	float summ = 0.0f;
	for(uint8_t i = 0; i < SPO2_QUEUE_SIZE; i++)
	{
		summ += spo2Queue[i];
	}
	return summ / (float)SPO2_QUEUE_SIZE;
}

float HrAvg()
{
	float summ = 0.0f;
	for(uint8_t i = 0; i < HR_QUEUE_SIZE; i++)
	{
		summ += hrQueue[i];
	}
	return summ / (float)HR_QUEUE_SIZE;
}

float Spo2Dev()
{
	float summ = 0.0f;
	for(uint8_t i = 0; i < SPO2_QUEUE_SIZE; i++)
	{
		summ += (spo2Queue[i] - Spo2Avg()) * (spo2Queue[i] - Spo2Avg());
	}
	return sqrt(summ / (float)SPO2_QUEUE_SIZE);
}

float HrDev()
{
	float summ = 0.0f;
	for(uint8_t i = 0; i < HR_QUEUE_SIZE; i++)
	{
		summ += (hrQueue[i] - HrAvg()) * (hrQueue[i] - HrAvg());
	}
	return sqrt(summ / (float)HR_QUEUE_SIZE);
}

void DoQueueMeas()
{
	Spo2Enqueue();
	HrEnqueue();
	
	
	Measurements[SPO2_DEV].value = Spo2Dev();
	Measurements[HR_DEV].value = HrDev();
	
	if (Spo2Dev() + HrDev() < 0.01f)
	{
		Measurements[SPO2_AVG].value = Spo2Avg();
		Measurements[HR_AVG].value = HrAvg();
	}
}