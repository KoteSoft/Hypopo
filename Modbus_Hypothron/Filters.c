/*
 * Filters.c
 *
 * Created: 02.11.2013 18:31:14
 *  Author: Слава
 */ 


float LowPassFilter(float x, float xp, float dT, float RC)
{
	float A = dT / (RC + dT);	
	return x * A + xp * (1.0 - A);	
}

float ModifyLowPassFilter(float x, float xp, float dT, float RC, float k)
{
	float A = dT / (RC + dT);
	return k * (x * A + xp * (1.0 - A));
}

float ComplementaryFilter(float x1, float x2, float k)
{
	return x1 * k + x2 * (1.0 - k);
}