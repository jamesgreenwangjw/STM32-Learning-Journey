/*
 * led.h
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include"main.h"
typedef enum {
    LED_OFF = 0,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW, // 红+绿
    LED_CYAN,   // 绿+蓝
    LED_PURPLE, // 红+蓝
    LED_WHITE   // 全亮
} LED_Color_t;

void LED_Set(LED_Color_t color);
void LED_Test_Loop(void); //这是我写的检测函数,开机跑一跑看看有没有毛病,后续可以注视掉

#endif /* INC_LED_H_ */
