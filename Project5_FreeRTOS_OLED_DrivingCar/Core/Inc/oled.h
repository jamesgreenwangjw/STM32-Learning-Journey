/*
 * oled.h
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */
#ifndef OLED_H_
#define OLED_H_
#include "main.h"

#define OLED_SCL_Port   GPIOB
#define OLED_SCL_Pin    GPIO_PIN_8
#define OLED_SDA_Port   GPIOB
#define OLED_SDA_Pin    GPIO_PIN_9

#define OLED_SCL_Clr() HAL_GPIO_WritePin(OLED_SCL_Port , OLED_SCL_Pin , GPIO_PIN_RESET)
#define OLED_SCL_Set() HAL_GPIO_WritePin(OLED_SCL_Port , OLED_SCL_Pin , GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(OLED_SDA_Port , OLED_SDA_Pin , GPIO_PIN_RESET)
#define OLED_SDA_Set() HAL_GPIO_WritePin(OLED_SDA_Port , OLED_SDA_Pin , GPIO_PIN_SET)

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x , uint8_t y , uint8_t chr);
void OLED_ShowString(uint8_t x , uint8_t y , uint8_t *chr);

void OLED_ShowChinese(uint8_t x , uint8_t y , uint8_t no);
void OLED_ShowPicture(uint8_t x , uint8_t y , uint8_t sizex , uint8_t sizey , uint8_t BMP[]);

#endif /* INC_OLED_H_ */
