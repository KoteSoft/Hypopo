/*
 * Params.c
 *
 * Created: 08.10.2013 11:27:24
 *  Author: Слава
 */ 
#include "Params.h"
#include "Output.h"
#include "main.h"
#include "Flow.h"
#include <avr/eeprom.h>
#include <stdbool.h>
#include "OxygenCalculations.h"
#include "ADC.h"
#include "GlobalConstants.h"

//curvepair_t Curve1[Curve_Size];
//curvepair_t Curve2[Curve_Size];
parametr_t Measurements[measurements_list_SIZE];
parametr_t savedParameters[saved_parameters_list_SIZE];
parametr_t nonsavedParameters[nonsaved_parameters_list_SIZE];

void ModbusEEPROMLoader();

uint8_t MbComm(uint16_t code);

uint16_t alarmMuteTimer = 0;

//Выводим телеметрию и т.п. в Inputs
void ModbusLoader()
{
	int i;
	for(i = 0; i < measurements_list_SIZE; i++)
	{
		usRegInputBuf[2 * i] = Measurements[i].array[0];
		usRegInputBuf[2 * i + 1] = Measurements[i].array[1];
	}	
}

//Обрабатываем значения HoldingRegisters
void ModbusSaver()
{
	/*Обработчики несохраняемых регистров прямого доступа*/
	//parametr_t temp;
	nonsavedParameters[O2_SET].array[0] = usRegHoldingBuf[O2_SET * 2];
	nonsavedParameters[O2_SET].array[1] = usRegHoldingBuf[O2_SET * 2 + 1];
	
	MbComm(usRegHoldingBuf[MB_COMMAND]);
	
	/*Сохранение новых значений регистров*/
	ModbusEEPROMLoader();
}


void ModbusInitValues()
{
	for(uint8_t i = 0; i < saved_parameters_list_SIZE; i++)
	{
		savedParameters[i].value = eeprom_read_float(EE_SAVED_PARAMS_OFFSET + 4 * i);
	}
	
	for (uint8_t i = 0; i < saved_parameters_list_SIZE; i++)
	{
		usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET + 0] = savedParameters[i].array[0];
		usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET + 1] = savedParameters[i].array[1];
	}
}

bool Uint32Comparrer(uint32_t A1, uint32_t A2, uint32_t B1, uint32_t B2)
{
	if (A1 != B1 || A2 != B2)
	{
		return false;
	}
	
	return true;
}

void ModbusEEPROMLoader()
{
	uint8_t sond_flag = 0;
	
	for (uint8_t i = 0; i < saved_parameters_list_SIZE; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET], usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET + 1], savedParameters[i].array[0], savedParameters[i].array[1]))
		{
			savedParameters[i].array[0] = usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET];
			savedParameters[i].array[1] = usRegHoldingBuf[2 * i + MB_SAVED_PARAMS_OFFSET + 1];
			eeprom_write_float(EE_SAVED_PARAMS_OFFSET + i * 4, savedParameters[i].value);
			sond_flag = 1;
		}
	}
	
	if (sond_flag)
	{
		for(int i = 0; i<100; i++)
		{
			Sound_On();
			_delay_us(500);
			Sound_Off();
			_delay_us(500);
		}
	}
	
	ModbusInitValues();
}

uint8_t MbComm(uint16_t code)
{
	usRegHoldingBuf[MB_COMMAND] = 0;
	switch (code)
	{
		case 0: 
		return 0; 
				
		case 3:
		return O2CoeffCalc();
		
		//Начать сеанс лечения
		case 4:
		{
			Measurements[DAMAGE].value = 0.0;
			Measurements[ALARM].value = 0.0;
			Measurements[DIAG].value = 0.0;
			Measurements[STATE].array[0] = DEVICE_THERAPY_STATE;
			return 0;
		}
		
		//Начать сеанс диагностики
		case 5:
		{
			Measurements[F_BR_START].value = Measurements[Fbreth].value;
			Measurements[HR_START].value = Measurements[HR_AVG].value;
			Measurements[STATE].array[0] = DEVICE_DIAGNOSTIC_STATE;
			Measurements[DAMAGE].value = 0.0;
			Measurements[ALARM].value = 0.0;
			Measurements[DIAG].value = 0.0;
			nonsavedParameters[O2_SET].value = 0.0;
			return 0;
		}
		
		//Завершить все и перейти в режим ожидания
		case 6:
		{
			Measurements[DAMAGE].value = 0.0;
			Measurements[ALARM].value = 0.0;
			Measurements[DIAG].value = 0.0;
			Measurements[STATE].array[0] = DEVICE_IDLE_STATE;
			nonsavedParameters[O2_SET].value = 0.0;
			return 0;
		}
		
		//Отключить сигнал
		case 7:
		{
			alarmMuteTimer = 3000;
			Measurements[DAMAGE].value = 0.0;
			Measurements[ALARM].value = 0.0;
			Measurements[DIAG].value = 0.0;
			Measurements[STATE].array[0] = DEVICE_THERAPY_STATE;
			return 0;
		}
		
		default:
		return 0;
	}
	
	return 0;
}

void HugeCalculations()
{
	Measurements[O2].value = Measurements[ADC4].value * nonsavedParameters[O2_K].value;
	Measurements[CO2].value = ((pow(10.0, ((Measurements[ADC3].value / savedParameters[K_AMP].value - savedParameters[EMF0].value) / savedParameters[DELTA_EMF].value) * (log10(400.0) - log10(1000.0)) + log10(400.0)))/10000);
	FlowCalc();
}