#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "main.h"

// 依赖的定时器句柄，需在 main.c 中定义
extern TIM_HandleTypeDef htim6;

void BSP_Delay_Init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms); 

#endif