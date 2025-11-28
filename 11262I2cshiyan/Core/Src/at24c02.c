/*
 * ST24c02.c
 *
 *  Created on: Nov 27, 2025
 *      Author: wangjunwei
 */

#include"at24c02.h"
#include"my_i2c.h"

int8_t AT24C02_WriteOneByte(uint8_t WriteAddr , uint8_t DataToWrite)
{
	I2C_Start();
	I2C_SendByte(0xA0);

	if (I2C_WaitAck() == 1)
	{
		I2C_Stop();
		return 1;
	}

	I2C_SendByte(WriteAddr);
	if (I2C_WaitAck() == 1)
	{
		I2C_Stop();
		return 1;
	}

	I2C_SendByte(DataToWrite);
	if (I2C_WaitAck() == 1)
	{
		I2C_Stop();
		return 1;
	}

	I2C_Stop();
	HAL_Delay(5);
	return 0;
}

int8_t AT24C02_ReadOneByte(uint8_t ReadAddr)
{
	volatile uint8_t receive;

	I2C_Start();

	I2C_SendByte(0xA0);
	if (I2C_WaitAck() == 1)
	{
		return -1;
	}

	I2C_SendByte(ReadAddr);
	if (I2C_WaitAck() == 1)
	{
		return -1;
	}

	I2C_Start();

	I2C_SendByte(0xA1);
	if (I2C_WaitAck() == 1)
	{
		return -1;
	}

	receive = I2C_ReadByte(0);

	I2C_Stop();
	HAL_Delay(5);
	return receive;
}





























