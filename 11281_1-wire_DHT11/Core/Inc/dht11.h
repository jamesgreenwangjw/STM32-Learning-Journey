/*
 * dht11.h
 *
 *  Created on: Nov 28, 2025
 *      Author: wangjunwei
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include"main.h"
#define Wire_L() HAL_GPIO_WritePin(GPIOG , GPIO_PIN_11 , GPIO_PIN_RESET)
#define Wire_H() HAL_GPIO_WritePin(GPIOG , GPIO_PIN_11 , GPIO_PIN_SET)
#define Read_Wire() HAL_GPIO_ReadPin(GPIOG , GPIO_PIN_11)

uint8_t Check(void);
uint8_t ReadOneByte(void);
uint8_t ReadData(uint8_t *buf);


#endif /* INC_DHT11_H_ */
