/*
 * delayus.c
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */
#include"main.h"
#include"tim.h"

void delay_us(uint32_t user_time)
{
	__HAL_TIM_SET_COUNTER(&htim4 , 0);
	HAL_TIM_Base_Start(&htim4);
	while(user_time > __HAL_TIM_GET_COUNTER(&htim4));
	HAL_TIM_Base_Stop(&htim4);
}

