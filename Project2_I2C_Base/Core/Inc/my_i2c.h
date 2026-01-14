/*
 * my_i2c.h
 *
 *  Created on: Nov 27, 2025
 *      Author: wangjunwei
 */

#ifndef INC_MY_I2C_H_
#define INC_MY_I2C_H_

#include"main.h"

#define I2C_SCL_H() HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_SET)
#define I2C_SCL_L() HAL_GPIO_WritePin(GPIOB , GPIO_PIN_6 , GPIO_PIN_RESET)

#define I2C_SDA_H() HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_SET)
#define I2C_SDA_L() HAL_GPIO_WritePin(GPIOB , GPIO_PIN_7 , GPIO_PIN_RESET)

#define I2C_READ_SCL() HAL_GPIO_ReadPin(GPIOB , GPIO_PIN_6)
#define I2C_READ_SDA() HAL_GPIO_ReadPin(GPIOB , GPIO_PIN_7)

void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_WaitAck(void);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_SendByte(uint8_t byte);
#endif /* INC_MY_I2C_H_ */
