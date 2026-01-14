/*
 * delayus.c
 *
 *  Created on: Dec 25, 2025
 *      Author: wangjunwei
 */
#include"main.h"
#include"tim.h"
#include"delayus.h"

void delay_us(uint32_t user_time)
{
	if(user_time == 0)
	{
		return;
	}
	htim6.Instance->CNT = 0;
	while(htim6.Instance->CNT < user_time);
}


