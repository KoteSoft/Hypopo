/*
 * Flow.c
 *
 * Created: 20.10.2013 19:02:51
 *  Author: Слава
 */ 
#include "Flow.h"
#include "Params.h"
#include "Filters.h"

float Qprev1, Qprev2; //значения ОРВ в предыдущем измерении 

//Кусочно-линейная интерполяция
float PLI(float X0, float Y0, float X1, float Y1, float X);
float PLI(float X0, float Y0, float X1, float Y1, float X)
{
	return Y0 + (Y1 - Y0) * ((X - X0) / (X1 - X0));
}

float Out1Calc(float A)
{
	float temp;
	Qprev1 = Measurements[Flow1].value;
	for (uint8_t i = 1; i < Curve_Size; i++)
	{
		if (A >= Curve1[i - 1].x.value && A <= Curve1[i].x.value)
		{
			temp = PLI(Curve1[i - 1].x.value, Curve1[i - 1].y.value, Curve1[i].x.value, Curve1[i].y.value, A);
			return ModifyLowPassFilter(temp, Qprev1, savedParameters[DT_F1].value, savedParameters[RC_F1].value, savedParameters[K_F1].value);
		}			
	}	
	
	return 0.;	
}

float Out2Calc(float A)
{
	float temp;
	Qprev2 = Measurements[Flow2].value;
	for (uint8_t i = 1; i < Curve_Size; i++)
	{
		if (A >= Curve2[i - 1].x.value && A <= Curve2[i].x.value)
		{
			temp = PLI(Curve2[i - 1].x.value, Curve2[i - 1].y.value, Curve2[i].x.value, Curve2[i].y.value, A);
			return ModifyLowPassFilter(temp, Qprev2, savedParameters[DT_F2].value, savedParameters[RC_F2].value, savedParameters[K_F2].value);
		}
	}
	
	return 0.;
}