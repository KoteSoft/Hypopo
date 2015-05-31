﻿/*
 * Params.h
 *
 * Created: 08.10.2013 11:27:12
 *  Author: Слава
 */ 


#ifndef PARAMS_H_
#define PARAMS_H_

#include "ParamsDef.h"

typedef enum
{
	Curve_Size = 30,
} sizes;

typedef enum
{
	/*Не сохраняются в EEPROM*/
	MB_O2_K				=	0,	//измеренный коэфициент О2->В
	MB_O2_SET			=	2,	//Заданная к поддержанию концентрация О2
	MB_AGE				=	3,	//Возраст пациента
	MB_STATE			=	4,	//Состояние/режим работы
	MB_PARAM6			=	5,
	MB_PARAM7			=	6,
	MB_PARAM8			=	7,
	MB_PARAM9			=	8,
	MB_PARAM10			=	9,
	MB_COMMAND			=	10,	
	/*Сохраняются в EEPROM*/
	MB_CURVE1X_OFFSET		=	20,
	MB_CURVE1Y_OFFSET		=	MB_CURVE1X_OFFSET + Curve_Size * 2,
	MB_CURVE2X_OFFSET		=	MB_CURVE1Y_OFFSET + Curve_Size * 2,
	MB_CURVE2Y_OFFSET		=	MB_CURVE2X_OFFSET + Curve_Size * 2,
	MB_SAVED_PARAMS_OFFSET	=	MB_CURVE2Y_OFFSET + Curve_Size * 2,
	MB_O2_K_MIN				=	MB_SAVED_PARAMS_OFFSET,			
	MB_O2_K_MAX				=	MB_O2_K_MIN + 2,
	MB_DT_F1				=	MB_O2_K_MAX + 2,
	MB_RC_F1				=	MB_DT_F1 + 2,
	MB_DT_F2				=	MB_RC_F1 + 2,
	MB_RC_F2				=	MB_DT_F2 + 2,
}modbus_holding_map;

typedef enum
{
	EE_CURVE1X_OFFSET		= 0,
	EE_CURVE1Y_OFFSET		= EE_CURVE1X_OFFSET + Curve_Size * 4,
	EE_CURVE2X_OFFSET		= EE_CURVE1Y_OFFSET + Curve_Size * 4,
	EE_CURVE2Y_OFFSET		= EE_CURVE2X_OFFSET + Curve_Size * 4,
	EE_SAVED_PARAMS_OFFSET	= EE_CURVE2Y_OFFSET + Curve_Size * 4,
}ee_map;

typedef enum
{
	O2	= 0,	//Содержание кислорода (измеренное)
	Flow1,	//Объемный расход воздуха - усиленный канал
	Flow2,	//Объемный расход воздуха - грубый канал
	FlowT,	//Объемный расход воздуха - общий
	Vin,	//Объем вдоха 
	Vout,	//Объем выдоха
	Fbreth,	//Частота дыхания
	ADC0,	//Усиленный канал
	ADC1,	//Грубый канал
	ADC2,	//Кислород
	ADC3,	//Датчик СО2
	ADC4,	
	ADC5,	
	ADC6,	
	ADC7,	
	HR,		//ЧСС
	SPO2,	//Сатурация крови
	CO2,
	HR_AVG,
	SPO2_AVG,
	HR_DEV,
	SPO2_DEV,
	HR_START,
	F_BR_START,
	STATE,
	ALARM,
	DAMAGE,
	DIAG,
	measurements_list_SIZE
}measurements_list;

/*Список параметров, которые сохраняются в EEPROM ПЗУ*/
typedef enum
{
	O2_K_MIN,	//Ограничение минимального значения коэффициента О2->В
	O2_K_MAX,	//Ограничение максимального значения коэффициента О2->В
	DT_F1,		//Константа времени RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	RC_F1,		//Коэффициент RC RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	K_F1,		//Коэффициент усиления RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	DT_F2,		//Константа времени RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	RC_F2,		//Коэффициент RC RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	K_F2,		//Коэффициент усиления RC-фильтра канала 1 - НЕ ИСПОЛЬЗУЕТСЯ
	OFF_F1,		//Смещение напряжения канала 1
	OFF_F2,		//Смещение напряжения канала 2 
	MINBR_FT,	//Минимальна скорость потока, которая регистрируетя как начало вдоха/выдоха
	SWBR_F2,	//Скорость потока, при которой расчет ОРВ переключается на грубый канал - НЕ ИСПОЛЬЗУЕТСЯ
	BR_V_MIN,	//Минимальный регистрируемый объем вдоха/выдоха, мл
	BR_V_MAX,	//Максимальный регистрируемый объем вдоха/выдоха, мл
	BR_T_MIN,	//Минимальное регистрируемое время вдоха/выдоха, мс
	BR_T_MAX,	//Максимальное регистрируемое время вдоха/выдоха, мс
	PT_FAN,		//П-коэффициент ПИДа длительности работы вентилятора
	IT_FAN,		//И-коэффициент ПИДа длительности работы вентилятора
	DT_FAN,		//Д-коэффициент ПИДа длительности работы вентилятора
	PI_FAN,		//П-коэффициент ПИДа скорости работы вентилятора
	II_FAN,		//И-коэффициент ПИДа скорости работы вентилятора
	DI_FAN,		//Д-коэффициент ПИДа скорости работы вентилятора
	FAN_MIN,	//Минимальная скорость вентилятора, расчитанная ПИДом, при которой вентилятор включается
	FAN_PID_T,	//Максимальное время работы вентилятора, которое может выдать ПИД
	CE,			//Коэффициент Се для расчета ОРВ по ГОСТ
	NLIN_FAN,	//Коэффициент нелинейности П-коэффициента ПИДа длительности работы вентилятора
	EMF0,		//ЕДС0 датчика СО2
	DELTA_EMF,	//Дельта-ЕДС датчика СО2
	K_AMP,		//Коэффициент усиления усилителя СО2
	saved_parameters_list_SIZE
}saved_parameters_list;

typedef enum
{
	O2_K,	//расчитанный коеффициент усиления для датчика кислорода
	O2_SET,	//заднное для поддержания содержание кислорода в контуре
	AGE,	//Возраст пациента
	nonsaved_parameters_list_SIZE
}nonsaved_parameters_list;

typedef enum
{
	DEVICE_IDLE_STATE,				
	DEVICE_THERAPY_STATE,
	DEVICE_DIAGNOSTIC_STATE,
	DEVICE_PROCEDURE_END_STATE,
	DEVICE_PROCEDURE_ALARM_STATE,
	DEVICE_DIAGNOSTIC_END_STATE,
	DEVICE_PROCEDURE_DAMAGE_STATE,
	device_states_list_size
}device_states_list;

extern parametr_t Measurements[measurements_list_SIZE];
extern parametr_t savedParameters[saved_parameters_list_SIZE];
extern parametr_t nonsavedParameters[nonsaved_parameters_list_SIZE];

void ModbusLoader();
void ModbusSaver();
void ModbusInitValues();
void HugeCalculations();

#endif /* PARAMS_H_ */