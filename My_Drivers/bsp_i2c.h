#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "main.h"
#include "bsp_delay.h"

// --- 用户配置区 (移植改这里) ---
#define I2C_SCL_PORT    GPIOB
#define I2C_SCL_PIN     GPIO_PIN_6
#define I2C_SDA_PORT    GPIOB
#define I2C_SDA_PIN     GPIO_PIN_7
// ----------------------------

#define I2C_SCL_H()     HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET)
#define I2C_SCL_L()     HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)
#define I2C_SDA_H()     HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET)
#define I2C_SDA_L()     HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)
#define I2C_SDA_READ()  HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN)

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_WaitAck(void);

#endif