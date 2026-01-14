#include "bsp_spi.h"

// 软件 SPI 初始化 (主要是设个空闲电平)
// GPIO 的模式配置建议在 CubeMX 里设为推挽输出
void SPI_Soft_Init(void)
{
    SPI_SCK_L();  // Mode 0 空闲时钟为低
    SPI_MOSI_H(); // 默认高
    // CS 引脚不在这里控制，由具体设备驱动控制
}

// 交换一个字节
// Mode 0: 在 SCL 下降沿(或初始)准备数据，在 SCL 上升沿读取数据
uint8_t SPI_SwapByte(uint8_t byte)
{
    uint8_t i;
    uint8_t rx_data = 0;

    for(i = 0; i < 8; i++)
    {
        // 1. 主机输出数据 (MOSI)
        if(byte & 0x80)
            SPI_MOSI_H();
        else
            SPI_MOSI_L();
        
        byte <<= 1;
        
        // 2. 拉高时钟 (上升沿，采样)
        SPI_SCK_H();
        
        // 3. 读取从机数据 (MISO)
        rx_data <<= 1;
        if(SPI_MISO_READ())
        {
            rx_data |= 0x01;
        }
        
        // 4. 拉低时钟 (下降沿，准备下一位)
        SPI_SCK_L();
    }
    
    return rx_data;
}
