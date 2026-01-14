/*
 * led.c
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */
#include"main.h"
#include"gpio.h"
#include"led.h"

static void LED_Clear(void)
{
    HAL_GPIO_WritePin(R_GPIO_Port , R_Pin , GPIO_PIN_SET);
    HAL_GPIO_WritePin(G_GPIO_Port , G_Pin , GPIO_PIN_SET);
    HAL_GPIO_WritePin(B_GPIO_Port , B_Pin , GPIO_PIN_SET);
}

void LED_Set(LED_Color_t color)
{
    LED_Clear();

    switch(color)
    {
        case LED_OFF:   break;

        case LED_RED:
            HAL_GPIO_WritePin(R_GPIO_Port , R_Pin , GPIO_PIN_RESET); break;

        case LED_GREEN:
            HAL_GPIO_WritePin(G_GPIO_Port , G_Pin , GPIO_PIN_RESET); break;

        case LED_BLUE:
            HAL_GPIO_WritePin(B_GPIO_Port , B_Pin , GPIO_PIN_RESET); break;

        case LED_YELLOW: // 红+绿 黄
            HAL_GPIO_WritePin(R_GPIO_Port , R_Pin , GPIO_PIN_RESET);
            HAL_GPIO_WritePin(G_GPIO_Port , G_Pin , GPIO_PIN_RESET); break;

        case LED_CYAN:   // 绿+蓝 cyan我上网搜了是蓝绿
            HAL_GPIO_WritePin(G_GPIO_Port , G_Pin , GPIO_PIN_RESET);
            HAL_GPIO_WritePin(B_GPIO_Port , B_Pin , GPIO_PIN_RESET); break;

        case LED_PURPLE: // 红+蓝 紫
            HAL_GPIO_WritePin(R_GPIO_Port , R_Pin , GPIO_PIN_RESET);
            HAL_GPIO_WritePin(B_GPIO_Port , B_Pin , GPIO_PIN_RESET); break;

        case LED_WHITE:
            HAL_GPIO_WritePin(R_GPIO_Port , R_Pin , GPIO_PIN_RESET);
            HAL_GPIO_WritePin(G_GPIO_Port , G_Pin , GPIO_PIN_RESET);
            HAL_GPIO_WritePin(B_GPIO_Port , B_Pin , GPIO_PIN_RESET); break;

        default: break;
    }
}

void LED_Test_Loop(void)
{
    LED_Set(LED_RED);   HAL_Delay(200);
    LED_Set(LED_GREEN); HAL_Delay(200);
    LED_Set(LED_BLUE);  HAL_Delay(200);
    LED_Set(LED_OFF);
}

