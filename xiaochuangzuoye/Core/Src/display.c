/*
 * display.c
 *
 *  Created on: Dec 25, 2025
 *      Author: wangjunwei
 */
#include"main.h"
#include"hc595.h"

const uint8_t SEG_TAB[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00};

void HC138_Select(uint8_t pos)
{
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10 , (pos & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_11 , (pos & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC , GPIO_PIN_12 , (pos & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void Display_Scan(uint8_t *buffer)
{
    static uint8_t pos = 0;
    uint8_t seg_code = 0;

    HC595_WriteByte(0x00);
    HC595_Latch();

    seg_code = SEG_TAB[buffer[pos]];
    if(pos == 1 || pos == 4)
    {
          seg_code |= 0x80;
    }
    HC595_WriteByte(seg_code);
    HC595_Latch();
    HC138_Select(pos);

    pos ++;
    if(pos >= 8) pos = 0;
}
