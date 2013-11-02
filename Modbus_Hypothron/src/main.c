/*
* Modbus_Hypothron.c
*
* Created: 10.09.2013 17:53:13
*  Author: Слава
*/


#include <main.h>
#include <Timer1.h>
#include <ADC.h>
#include <Params.h>
#include <GlobalConstants.h>

USHORT   usRegInputBuf[REG_INPUT_NREGS];
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

unsigned char ucRegCoilsBuf[REG_COILS_SIZE / 8];

int main(void)
{
    DDRA = 0x00;
    DDRB = 1<<PORTB0|1<<PORTB1|1<<PORTB2|1<<PORTB3|1<<PORTB4|1<<PORTB5|1<<PORTB6|1<<PORTB7;
    DDRC = 1<<PORTC0|1<<PORTC1|1<<PORTC2|0<<PORTC3|0<<PORTC4|0<<PORTC5|0<<PORTC6|0<<PORTC7;
    DDRD = 0<<PORTD0|0<<PORTD1|1<<PORTD2|0<<PORTD3|1<<PORTD4|0<<PORTD5|1<<PORTD6|0<<PORTD7;
    
    Timer1_Init(10);
    
    sei();
    
    eMBInit( MB_RTU, 0x01, 0, UART_BAUD_RATE, MB_PAR_NONE );
    eMBEnable();
    
    /*Загружаем в Holding Registers и в массив параметров значения из EEPROM*/
    ModbusInitValues();
    
    ADC_Init();
    
    while(1)
    {
        /*Актуализируем значения Modbus-регистров в соответствии со значениями параметров*/
        ModbusLoader();
        /*Актуализируем значения параметров в соответствии со значениями Holding Registers*/
        ModbusSaver();
        Get_ADC();
        HugeCalculations();
        /*Итерация Modbus*/
        eMBPoll();
    }
    
}

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
    && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
            ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
            ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
    ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
            case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
            * protocol stack. */
            case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}



eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}