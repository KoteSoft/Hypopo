﻿/*
 * ADC.h
 *
 * Created: 08.10.2013 0:07:25
 *  Author: Слава
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

volatile uint16_t ADC_Result[8];
void ADC_Init();
void Get_ADC();

#endif /* ADC_H_ */