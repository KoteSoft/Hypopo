/*
* OxygenCalculations.c
*
* Created: 30.10.2013 20:33:49
*  Author: Слава
*/
#include <hypopo/OxygenCalculations.h>
#include <hypopo/ADC.h>
#include <hypopo/Params.h>
#include <hypopo/GlobalConstants.h>
#include <hypopo/main.h>

uint8_t O2CoeffCalc()
{
    Parameters[O2_K].value = O2_in_air / Measurements[ADC2].value;
    usRegHoldingBuf[MB_O2_K] = Parameters[O2_K].array[0];
    usRegHoldingBuf[MB_O2_K] = Parameters[O2_K].array[1];
    
    if (Parameters[O2_K].value > Parameters[O2_K_MIN].value && Parameters[O2_K].value < Parameters[O2_K_MAX].value)
    {
        return 5;
    }
    
    return 1;
}