/*
 * hc595.c
 *
 *  Created on: Dec 25, 2025
 *      Author: wangjunwei
 */
#include "main.h"

#define HC595_DATA_H()  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_3 , GPIO_PIN_SET)
#define HC595_DATA_L()  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_3 , GPIO_PIN_RESET)

#define HC595_SCK_H()   HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_SET)
#define HC595_SCK_L()   HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5 , GPIO_PIN_RESET)

#define HC595_LCLK_H()  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_SET)
#define HC595_LCLK_L()  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_RESET)

void HC595_WriteByte(uint8_t byte)
{
    uint8_t i;
    for(i = 0 ; i < 8 ; i ++)
    {

        if(byte & 0x80)
            HC595_DATA_H();
        else
            HC595_DATA_L();

        HC595_SCK_L();
        HC595_SCK_H();

        byte <<= 1;
    }
}

void HC595_Latch(void)
{
    HC595_LCLK_L();
    HC595_LCLK_H();
}
