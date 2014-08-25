/*
 * CMS50L.c
 *
 * Created: 22.08.2014 0:41:59
 *  Author: Слава
 */ 
#include "CMS50L.h"
#include "i2c.h"
#include "Params.h"
#include <util/twi.h>
#include <avr/interrupt.h>

volatile uint8_t i2c_state = 0;

uint8_t CMS50L_Poll()
{
	/*
	twi(TWI_START);
	TWDR = CMS50L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x0A;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR = CMS50L_ADDR | I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	twi(TWI_RECEIVE_ACK);
	twi(TWI_RECEIVE_NACK);
	twi(TWI_STOP);
	*/
	/*
	twi(TWI_START);
	TWDR = CMS50L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x00;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR = CMS50L_ADDR | I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	Measurements[HR].value = TWDR;
	twi(TWI_RECEIVE_NACK);
	Measurements[SPO2].value = TWDR;
	twi(TWI_STOP);
	*/
	return 0;
}

ISR(TWI_vect, ISR_BLOCK)
{
	uint8_t i2c_status = TWSR & 0xF8;
	
	switch (i2c_status)
	{
		case 0x08:	//Start
		{
			if (i2c_state == 0)
			{
				TWDR = CMS50L_ADDR | I2C_WRITE;
				twi(TWI_TRANSMIT);
				i2c_state = 1;
			}
			break;
		}
		
		case 0x18:
		{
			if (i2c_state == 1)
			{
				TWDR = 0x00;
				twi(TWI_TRANSMIT);
				i2c_state = 2;
			}
			break;
		}
		
		case 0x28:
		{
			if (i2c_state == 2)
			{
				twi(TWI_RESTART);
				i2c_state = 3;
			}
			break;
		}
		
		case 0x10:
		{
			if (i2c_state == 3)
			{
				TWDR = CMS50L_ADDR | I2C_READ;
				twi(TWI_TRANSMIT);
				i2c_state = 4;
			}
			else if (i2c_state == 0)
			{
				TWDR = CMS50L_ADDR | I2C_WRITE;
				twi(TWI_TRANSMIT);
				i2c_state = 1;
			}
			break;
		}
		
		case 0x40:
		{
			if (i2c_state == 4)
			{
				twi(TWI_RECEIVE_ACK);				
				i2c_state = 5;
			}
			break;
		}
		
		case 0x50:
		{
			if (i2c_state == 5)
			{
				Measurements[HR].value = TWDR;
				twi(TWI_RECEIVE_NACK);
				i2c_state = 6;
			}
			break;
		}
		
		case 0x58:
		{
			if (i2c_state == 6)
			{				
				Measurements[SPO2].value = TWDR;
				twi(TWI_RESTART);
				i2c_state = 0;
			}
			break;
		}
		
		default:
		{
			twi(TWI_RESTART);
			i2c_state = 0;
		}
	}
}