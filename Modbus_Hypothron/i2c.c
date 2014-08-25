/*
 * i2c.c
 *
 * Created: 22.08.2014 0:28:26
 *  Author: Слава
 */ 
#include "i2c.h"
#include <avr/interrupt.h>
#include "Params.h"

void twi(uint8_t action)
{
	switch(action)
	{
		case TWI_START:
		case TWI_RESTART:
		TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
		break;
		case TWI_STOP:
		TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
		break;
		case TWI_TRANSMIT:
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
		break;
		case TWI_RECEIVE_ACK:
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA) | _BV(TWIE);
		break;
		case TWI_RECEIVE_NACK:
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
		break;
	}
	/*
	if(action != TWI_STOP)
	while (!(TWCR & _BV(TWINT)));
	return (TWSR & 0xF8);
	*/
}

void i2c_Init()
{
	TWBR = ((F_CPU / I2C_SPEED) - 16) / 2;
	twi(TWI_START);
}


