﻿/*
 * i2c.h
 *
 * Created: 22.08.2014 0:28:39
 *  Author: Слава
 */ 



#ifndef I2C_H_
#define I2C_H_


#include <avr/io.h>
#include <util/delay.h>

#define I2C_SPEED			400L

#define TWI_START			0
#define TWI_RESTART         1
#define TWI_STOP            2
#define TWI_TRANSMIT        3
#define TWI_RECEIVE_ACK     4
#define TWI_RECEIVE_NACK    5

#define I2C_WRITE			0b00000000
#define I2C_READ			0b00000001

void twi(uint8_t action);
void i2c_Init();


typedef enum
{
	BMP085_Response_Fail	= 0b00000001,
	ADXL345_Response_Fail	= 0b00000010,
	L3G4200D_Response_Fail	= 0b00000100,
	HMC5883L_Response_Fail	= 0b00001000
	
} Sensor_errors_list;

typedef enum
{
	Bus_Fail			= 0x00,	//Автобус сломался… эээ в смысле аппаратная ошибка шины. Например, внезапный старт посреди передачи бита
	Start				= 0x08,	//Был сделан старт. Теперь мы решаем что делать дальше, например послать адрес ведомого
	ReStart				= 0x10,	//Был обнаружен повторный старт. Можно переключиться с записи на чтение или наоборот. От логики зависит.
	SLA_W_ACK			= 0x18,	//Мы отправили адрес с битом записи, а в ответ получили ACK от ведомого. Значит можно продолжать.
	SLA_W_NACK			= 0x20,	//Мы отправили адрес с битом записи, а нас послали NACK. Обидно, сгенерим ошибку или повторим еще раз.
	Byte_ACK			= 0x28,	//Мы послали байт и получили подтверждение, что ведомый его принял. Продолжаем.
	Byte_NACK			= 0x30,	//Мы послали байт, но подтверждение не получили. Видимо ведомый уже сыт по горло нашими подачками или он захлебнулся в данных. Либо его ВНЕЗАПНО посреди передачи данных украли инопланетяне.
	Collision			= 0x38,	//А у нас тут клановые разборки — пришел другой мастер, по хамски нас перебил, да так, что мы от возмущения аж заткнулись. Ничего I’l be back! До встречи через n тактов!
	SLA_R_ACK			= 0x40,	//Послали адрес с битом на чтение, а ведомый отозвался. Хорошо! Будем читать.
	SLA_R_NACK			= 0x48,	//Крикнули в шину «Эй ты, с адресом ХХХ, почитай нам сказки» А в ответ «Иди NACK!» В смысле на запрос адреса с битом чтения никто не откликнулся. Видимо не хотят или заняты. Также может быть никого нет дома.
	Recive_Byte			= 0x50,	//Мы приняли байт. И думаем что бы ответить ведомому. ACK или NACK.
	Recive_Byte_NACK	= 0x58,	//Мы приняли байт от ведомого и сказали ему «иди NACK!» И он обиженый ушел, освободив шину.
	Stop				= 0xF8	//Стоп линии
} i2c_Master_status;

#endif /* I2C_H_ */