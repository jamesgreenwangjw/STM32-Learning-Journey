#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "main.h"

// --- 用户配置区 ---
#define SPI_SCK_PORT    GPIOB
#define SPI_SCK_PIN     GPIO_PIN_13
#define SPI_MOSI_PORT   GPIOB
#define SPI_MOSI_PIN    GPIO_PIN_15
#define SPI_MISO_PORT   GPIOB
#define SPI_MISO_PIN    GPIO_PIN_14
// CS 引脚通常由具体设备驱动控制，不放在这里
// ----------------

#define SPI_SCK_H()     HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET)
#define SPI_SCK_L()     HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET)
#define SPI_MOSI_H()    HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET)
#define SPI_MOSI_L()    HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET)
#define SPI_MISO_READ() HAL_GPIO_ReadPin(SPI_MISO_PORT, SPI_MISO_PIN)

void SPI_Soft_Init(void);
uint8_t SPI_SwapByte(uint8_t byte);

#endif