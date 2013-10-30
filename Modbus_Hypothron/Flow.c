/*
 * Flow.c
 *
 * Created: 20.10.2013 19:02:51
 *  Author: Слава
 */ 
#include "Flow.h"
#include "Params.h"

//Кусочно-линейная интерполяция
float PLI(float X0, float Y0, float X1, float Y1, float X);
float PLI(float X0, float Y0, float X1, float Y1, float X)
{
	return Y0 + (Y1 - Y0) * ((X - X0) / (X1 - X0));
}

float Out1Calc(float A)
{
	for (uint8_t i = 1; i < Curve_Size; i++)
	{
		if (A >= Curve1[i - 1].x.value && A <= Curve1[i].x.value)
		{
			return PLI(Curve1[i - 1].x.value, Curve1[i - 1].y.value, Curve1[i].x.value, Curve1[i].y.value, A);
		}			
	}	
	
	return 0.;	
}

float Out2Calc(float A)
{
	for (uint8_t i = 1; i < Curve_Size; i++)
	{
		if (A >= Curve2[i - 1].x.value && A <= Curve2[i].x.value)
		{
			return PLI(Curve2[i - 1].x.value, Curve2[i - 1].y.value, Curve2[i].x.value, Curve2[i].y.value, A);
		}
	}
	
	return 0.;
}