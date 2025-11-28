/*
 * my_i2c.c
 *
 *  Created on: Nov 27, 2025
 *      Author: wangjunwei
 */
#include"main.h"
#include"my_i2c.h"

/*I2C开始传输数据是scl高电平的模式下,sda电平由高变低,
 * 所以应该是先保证scl电平高
 * 然后将sda电平拉低
 */
static void I2C_Delay(void)
{
	volatile int i = 20;
	while(i --)
	{
	}
}
void I2C_Start(void)
{
	I2C_SCL_H();
	I2C_SDA_H();
	I2C_Delay();

	I2C_SDA_L();
	I2C_Delay();

	I2C_SCL_L();
}

void I2C_Stop(void)
{
	I2C_SCL_L();
	I2C_SDA_L();
	I2C_Delay();

	I2C_SCL_H();
	I2C_Delay();

	I2C_SDA_H();
	I2C_Delay();
}

uint8_t I2C_WaitAck(void)
{
	/*先释放SDA
	 * 再拉高SCL准备读SDA
	 * 然后判断是否SDA被拉低
	 */
	volatile uint8_t error_time = 0;
	I2C_SDA_H();
	I2C_Delay();

	I2C_SCL_H();
	I2C_Delay();
	error_time = 0;

	while(1)
	{
		if(error_time >= 40)
		{
			I2C_SCL_L();
			return 1;
		}

		if(I2C_READ_SDA() == 0)
		{
			I2C_SCL_L();
			return 0;
		}

		error_time ++;
	}
}

void I2C_SendByte(uint8_t byte)
{
	uint8_t i;
	for(i = 0 ; i < 8 ; i ++)
	{
		I2C_SCL_L();
		I2C_Delay();

		if(byte & 0x80)
		{
			I2C_SDA_H();
		}
		else if((byte & 0x80) == 0)
		{
			I2C_SDA_L();
		}

		I2C_Delay();

		I2C_SCL_H();
		I2C_Delay();

		byte <<= 1;
	}
	I2C_SCL_L();
}

uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i = 0;
	volatile uint8_t receive = 0;
	I2C_SDA_H();
	I2C_Delay();

	for(i = 0 ; i < 8 ; i ++)
	{
		I2C_SCL_L();
		I2C_Delay();

		I2C_SCL_H();
		I2C_Delay();

		receive <<= 1;
		if(I2C_READ_SDA() == 1)
		{
			receive |= 0x01;
		}

		else if(I2C_READ_SDA() == 0)
		{
		}

		I2C_Delay();
	}

	I2C_SCL_L();

	if(ack)
	{
		I2C_SDA_L();
	}
	else
	{
		I2C_SDA_H();
	}

	I2C_Delay();

	I2C_SCL_H();
	I2C_Delay();

	I2C_SCL_L();

	return receive;
}


















