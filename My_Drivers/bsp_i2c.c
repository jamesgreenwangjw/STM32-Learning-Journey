#include "bsp_i2c.h"

// I2C 起始信号
void I2C_Start(void)
{
    I2C_SDA_H();
    I2C_SCL_H();
    delay_us(2);
    I2C_SDA_L();
    delay_us(2);
    I2C_SCL_L(); // 钳住总线，准备发送
}

// I2C 停止信号
void I2C_Stop(void)
{
    I2C_SCL_L();
    I2C_SDA_L();
    delay_us(2);
    I2C_SCL_H();
    delay_us(2);
    I2C_SDA_H(); // 发送结束信号
    delay_us(2);
}

// 发送一个字节
void I2C_SendByte(uint8_t byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        I2C_SCL_L(); // 拉低时钟开始数据变化
        
        if(byte & 0x80)
        {
            I2C_SDA_H();
        }
        else
        {
            I2C_SDA_L();
        }
        
        delay_us(2);
        I2C_SCL_H(); // 拉高时钟让从机读
        delay_us(2);
        I2C_SCL_L(); // 拉低准备下一位
        
        byte <<= 1;
    }
}

// 等待应答
// 返回值：0-应答(ACK), 1-非应答(NACK)
uint8_t I2C_WaitAck(void)
{
    uint8_t ack_status = 0;
    
    I2C_SCL_L();
    I2C_SDA_H(); // 主机释放总线（切换为输入）
    delay_us(2);
    I2C_SCL_H(); // 拉高时钟准备读
    delay_us(2);
    
    if(I2C_SDA_READ())
    {
        ack_status = 1; // 读到高电平，说明没人拉低，NACK
    }
    else
    {
        ack_status = 0; // 读到低电平，ACK
    }
    
    I2C_SCL_L();
    return ack_status;
}