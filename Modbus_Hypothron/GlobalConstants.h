/*
 * GlobalConstants.h
 *
 * Created: 29.10.2013 11:12:59
 *  Author: Слава
 */ 


#ifndef GLOBALCONSTANTS_H_
#define GLOBALCONSTANTS_H_

#include <stdint.h>

extern const float O2_in_air; //%кислорода в воздухе
extern const float U_ref; //опорное напряжение АЦП	
extern const uint16_t H_Step; //время дискретизации
extern const float U0; //Напряжение на выходе усилителя ОРВ при отсутствии потока
extern const float Rho; //Плотность воздуха

#endif /* GLOBALCONSTANTS_H_ */