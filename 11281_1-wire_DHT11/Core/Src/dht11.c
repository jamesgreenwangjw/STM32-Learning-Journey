/*
 * dht11.c
 *
 *  Created on: Nov 28, 2025
 *      Author: wangjunwei
 */
#include"main.h"
#include"dht11.h"
#include"delay_us.h"

uint8_t Check(void)
{
	Wire_L();
	HAL_Delay(20);

	Wire_H();
	delay_us(30);

	if(Read_Wire() == 0)
	{
		while(Read_Wire() == 0)
		{
		}
		while(Read_Wire() == 1)
		{
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t ReadOneByte(void)
{
	uint8_t receive;
	volatile int i = 0;
	uint8_t try = 0;
	for(i = 0 ; i < 8 ; i ++)
	{
		while(Read_Wire() == 0 && try < 100)
		{
			try ++;
			delay_us(1);

		}

		delay_us(40);

		receive <<= 1;

		if(Read_Wire() == 0)
		{
		}

		else if(Read_Wire() == 1)
		{
			receive += 1;
		}

		while(Read_Wire() == 1)
		{
		}


	}

	return receive;
}

uint8_t ReadData(uint8_t *buf)
{
	volatile uint8_t i = 0;
	if(Check() == 1)
	{
		return 0xFF;
	}

	for(i = 0 ; i < 5 ; i ++)
	{
		buf[i] = ReadOneByte();
	}

	if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
	{
		return 0;
	}

	else
	{
		return 1;
	}
}





























