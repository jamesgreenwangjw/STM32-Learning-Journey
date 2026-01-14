/*
 * DHT11.c
 *
 *  Created on: Dec 25, 2025
 *      Author: wangjunwei
 */
#include "main.h"
#include "tim.h"
#include "delayus.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


#define DHT11_READ_IO() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)
void DHT11_Mode_Out_PP(void)
{

    GPIOB->CRH &= ~(0xF << 4);
    GPIOB->CRH |= (0x3 << 4);
}

void DHT11_Mode_In(void)
{

    GPIOB->CRH &= ~(0xF << 4);
    GPIOB->CRH |= (0x8 << 4);
}

void DHT11_Set_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void DHT11_Set_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint8_t Check()
{
	uint8_t retry = 0;

	DHT11_Set_Output();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	osDelay(20);
	taskENTER_CRITICAL();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	DHT11_Set_Input();
	delay_us(30);

	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
	{
		retry++;
		delay_us(1);
		if(retry > 100) return 1;
	}

	retry = 0;
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET)
	{
		retry++;
		delay_us(1);
		if(retry > 100) return 1;
	}

	retry = 0;
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
	{
		retry++;
		delay_us(1);
		if(retry > 100) return 1;
	}

	return 0;
}

uint8_t SendOneByte()
{
	uint8_t sen_byte = 0;
	uint8_t i;
	uint16_t timeout = 0;

	for(i = 0 ; i < 8 ; i ++)
	{
		timeout = 0;
		while(DHT11_READ_IO() == GPIO_PIN_RESET)
		{
			timeout++;
			if(timeout > 5000) return 0xFF;
		}
		__HAL_TIM_SET_COUNTER(&htim6, 0);

		timeout = 0;
		while(DHT11_READ_IO() == GPIO_PIN_SET)
		{
			timeout++;
			if(timeout > 5000) return 0xFF;
		}

		if(__HAL_TIM_GET_COUNTER(&htim6) > 40)
		{
			sen_byte = (sen_byte << 1) | 0x01;
		}
		else
		{
			sen_byte <<= 1;
		}
	}
	return sen_byte;
}

uint8_t ReadByte(uint8_t *rx_buf)
{
	uint8_t check_res = Check();

	if(check_res != 0)
	{
		taskEXIT_CRITICAL();
		return 1;
	}

	for(int i = 0 ; i < 5 ; i ++)
	{
	    rx_buf[i] = SendOneByte();

		if(rx_buf[i] == 0xFF)
		{
			taskEXIT_CRITICAL();
			return 1;
		}
	}

	taskEXIT_CRITICAL();

	if((uint8_t)(rx_buf[0] + rx_buf[1] + rx_buf[2] + rx_buf[3]) == rx_buf[4])
	{
		return 0;
	}
	else
	{
		return 2;
	}
}
