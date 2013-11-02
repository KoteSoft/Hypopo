/*
 * Filters.h
 *
 * Created: 02.11.2013 18:31:00
 *  Author: Слава
 */ 


#ifndef FILTERS_H_
#define FILTERS_H_

float LowPassFilter(float x, float xp, float dT, float RC);
float ModifyLowPassFilter(float x, float xp, float dT, float RC, float k);
float ComplementaryFilter(float x1, float x2, float k);

#endif /* FILTERS_H_ */