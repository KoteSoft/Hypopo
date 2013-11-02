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

curvepair_t Curve1[Curve_Size];
curvepair_t Curve2[Curve_Size];
parametr_t Measurements[measurements_list_SIZE];
parametr_t savedParameters[saved_parameters_list_SIZE];
parametr_t nonsavedParameters[nonsaved_parameters_list_SIZE];

void ModbusEEPROMLoader();

uint8_t MbComm(uint16_t code);

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
	parametr_t temp;
	temp.array[0] = usRegHoldingBuf[MB_PARAM3];
	temp.array[1] = usRegHoldingBuf[MB_PARAM4];
	temp.value = Out1Calc(temp.value);
	usRegHoldingBuf[MB_PARAM5] = temp.array[0];
	usRegHoldingBuf[MB_PARAM6] = temp.array[1]; 
	
	MbComm(usRegHoldingBuf[MB_COMMAND]);
	
	/*Сохранение новых значений регистров*/
	ModbusEEPROMLoader();
}


void ModbusInitValues()
{
	for(uint8_t i = 0; i < Curve_Size; i++)
	{
		Curve1[i].x.value = eeprom_read_float(EE_CURVE1X_OFFSET + 4 * i);
		Curve1[i].y.value = eeprom_read_float(EE_CURVE1Y_OFFSET + 4 * i);		
		Curve2[i].x.value = eeprom_read_float(EE_CURVE2X_OFFSET + 4 * i);
		Curve2[i].y.value = eeprom_read_float(EE_CURVE2Y_OFFSET + 4 * i);
	}
	
	for(uint8_t i = 0; i < saved_parameters_list_SIZE; i++)
	{
		savedParameters[i].value = eeprom_read_float(EE_SAVED_PARAMS_OFFSET + 4 * i);
	}

	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET + 0] = Curve1[i].x.array[0];
		usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET + 1] = Curve1[i].x.array[1];
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET + 0] = Curve1[i].y.array[0];
		usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET + 1] = Curve1[i].y.array[1];
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET + 0] = Curve2[i].x.array[0];
		usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET + 1] = Curve2[i].x.array[1];
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET + 0] = Curve2[i].y.array[0];
		usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET + 1] = Curve2[i].y.array[1];
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

	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET], usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET + 1], Curve1[i].x.array[0], Curve1[i].x.array[1]))
		{
			Curve1[i].x.array[0]=usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET];
			Curve1[i].x.array[1]=usRegHoldingBuf[2 * i + MB_CURVE1X_OFFSET + 1];
			eeprom_write_float(EE_CURVE1X_OFFSET + i * 4, Curve1[i].x.value);
			sond_flag = 1;
		}
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET], usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET + 1], Curve1[i].y.array[0], Curve1[i].y.array[1]))
		{
			Curve1[i].y.array[0]=usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET];
			Curve1[i].y.array[1]=usRegHoldingBuf[2 * i + MB_CURVE1Y_OFFSET + 1];
			eeprom_write_float(EE_CURVE1Y_OFFSET + i * 4, Curve1[i].y.value);
			sond_flag = 1;
		}
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET], usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET + 1], Curve2[i].x.array[0], Curve2[i].x.array[1]))
		{
			Curve2[i].x.array[0]=usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET];
			Curve2[i].x.array[1]=usRegHoldingBuf[2 * i + MB_CURVE2X_OFFSET + 1];
			eeprom_write_float(EE_CURVE2X_OFFSET + i * 4, Curve2[i].x.value);
			sond_flag = 1;
		}
	}
	
	for (uint8_t i = 0; i < Curve_Size; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET], usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET + 1], Curve2[i].y.array[0], Curve2[i].y.array[1]))
		{
			Curve2[i].y.array[0]=usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET];
			Curve2[i].y.array[1]=usRegHoldingBuf[2 * i + MB_CURVE2Y_OFFSET + 1];
			eeprom_write_float(EE_CURVE2Y_OFFSET + i * 4, Curve2[i].y.value);
			sond_flag = 1;
		}
	}

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
		Sound_On();
		_delay_ms(100);
		Sound_Off();
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
		
		default:
		return 0;
	}
	
	return 0;
}

void HugeCalculations()
{
	Measurements[O2].value = Measurements[ADC2].value * nonsavedParameters[O2_K].value;
	Measurements[Flow1].value = Out1Calc(Measurements[ADC0].value * 2.0 - 5.0);
	Measurements[Flow2].value = Out2Calc(Measurements[ADC1].value * 2.0 - 5.0);
	
	
}