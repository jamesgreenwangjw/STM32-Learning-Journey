/*
 * delay_us.c
 *
 *  Created on: Nov 28, 2025
 *      Author: wangjunwei
 */

#include"delay_us.h"
#include"main.h"
#include"tim.h"

extern TIM_HandleTypeDef htim6;

void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim6 , 0);
	HAL_TIM_Base_Start(&htim6);


	while(__HAL_TIM_GET_COUNTER(&htim6) < us)
	{
	}

	HAL_TIM_Base_Stop(&htim6);
}




