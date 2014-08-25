/*
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
	MB_O2_SET			=	2,
	MB_PARAM4			=	3,
	MB_PARAM5			=	4,
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
	measurements_list_SIZE
}measurements_list;

typedef enum
{
	O2_K_MIN,
	O2_K_MAX,
	DT_F1,
	RC_F1,
	K_F1,
	DT_F2,
	RC_F2,
	K_F2,
	OFF_F1,
	OFF_F2,
	MINBR_FT,
	SWBR_F2,
	BR_V_MIN,
	BR_V_MAX,
	BR_T_MIN,
	BR_T_MAX,
	PT_FAN,
	IT_FAN,
	DT_FAN,
	PI_FAN,
	II_FAN,
	DI_FAN,
	FAN_MIN,
	FAN_PID_T,
	CE,
	NLIN_FAN,
	saved_parameters_list_SIZE
}saved_parameters_list;

typedef enum
{
	O2_K,	//расчитанный коеффициент усиления для датчика кислорода
	O2_SET,	//заднное для поддержания содержание кислорода в контуре
	nonsaved_parameters_list_SIZE
}nonsaved_parameters_list;

extern curvepair_t Curve1[Curve_Size];
extern curvepair_t Curve2[Curve_Size];
extern parametr_t Measurements[measurements_list_SIZE];
extern parametr_t savedParameters[saved_parameters_list_SIZE];
extern parametr_t nonsavedParameters[nonsaved_parameters_list_SIZE];

void ModbusLoader();
void ModbusSaver();
void ModbusInitValues();
void HugeCalculations();

#endif /* PARAMS_H_ */