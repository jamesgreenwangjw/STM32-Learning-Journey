/*
 * oled.c
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */
// delay us 还没写别忘了
//
//
//
//
//
//
#include"oled.h"
#include"oledfont.h"
#include"delayus.h"

static void I2C_Start(void)
{
    OLED_SDA_Set();
    OLED_SCL_Set();
    delay_us(2);
    OLED_SDA_Clr();
    OLED_SCL_Clr();
}

static void I2C_Stop(void)
{
    OLED_SDA_Clr();
    OLED_SCL_Set();
    OLED_SDA_Set();
}

static void I2C_WaitAck(void)
{
    OLED_SDA_Set();
    OLED_SCL_Set();
    OLED_SCL_Clr();
}

static void Send_Byte(uint8_t dat)
{
    uint8_t i;
    for(i = 0 ; i < 8 ; i++)
    {
        OLED_SCL_Clr();
        if(dat & 0x80)
        {
            OLED_SDA_Set();
        }
        else
        {
            OLED_SDA_Clr();
        }
        OLED_SCL_Set();
        OLED_SCL_Clr();
        dat <<= 1;
    }
}

static void OLED_WrCmd(uint8_t cmd)
{
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    Send_Byte(0x00);
    I2C_WaitAck();
    Send_Byte(cmd);
    I2C_WaitAck();
    I2C_Stop();
}

static void OLED_WrDat(uint8_t dat)
{
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    Send_Byte(0x40);
    I2C_WaitAck();
    Send_Byte(dat);
    I2C_WaitAck();
    I2C_Stop();
}

void OLED_Init(void)
{
    HAL_Delay(100); // 上电等待

    OLED_WrCmd(0xAE); // 关闭显示
    OLED_WrCmd(0xD5); // 设置时钟分频因子
    OLED_WrCmd(0x80);
    OLED_WrCmd(0xA8); // 设置驱动路数
    OLED_WrCmd(0x3F); // 默认0x3F(1/64)
    OLED_WrCmd(0xD3); // 设置显示偏移
    OLED_WrCmd(0x00);
    OLED_WrCmd(0x40); // 设置显示开始行
    OLED_WrCmd(0x8D); // 电荷泵设置
    OLED_WrCmd(0x14); // 开启电荷泵
    OLED_WrCmd(0x20); // 设置内存地址模式
    OLED_WrCmd(0x02); // 页地址模式
    OLED_WrCmd(0xA1); // 段重定义设置
    OLED_WrCmd(0xC8); // 设置COM扫描方向
    OLED_WrCmd(0xDA); // 设置COM硬件引脚配置
    OLED_WrCmd(0x12);
    OLED_WrCmd(0x81); // 对比度设置
    OLED_WrCmd(0xEF);
    OLED_WrCmd(0xD9); // 设置预充电周期
    OLED_WrCmd(0xF1);
    OLED_WrCmd(0xDB); // 设置VCOMH取消选择级别
    OLED_WrCmd(0x30);
    OLED_WrCmd(0xA4); // 全局显示开启
    OLED_WrCmd(0xA6); // 设置正常显示
    OLED_WrCmd(0xAF); // 开启显示
    OLED_Clear();
}

void OLED_Clear(void)
{
    uint8_t i , n;
    for(i = 0 ; i < 8 ; i++)
    {
        OLED_WrCmd(0xB0 + i);
        OLED_WrCmd(0x00);
        OLED_WrCmd(0x10);
        for(n = 0 ; n < 128 ; n++)
        {
            OLED_WrDat(0);
        }
    }
}

void OLED_ShowChar(uint8_t x , uint8_t y , uint8_t chr)
{
    unsigned char c = 0 , i = 0;
    c = chr - ' ';
    if(x > 120)
    {
        x = 0;
        y = y + 2;
    }

    OLED_WrCmd(0xB0 + y);
    OLED_WrCmd(((x & 0xF0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0F));

    for(i = 0 ; i < 8 ; i++)
    {
        OLED_WrDat(OLED_F8x16[c][i]);
    }

    OLED_WrCmd(0xB0 + y + 1);
    OLED_WrCmd(((x & 0xF0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0F));

    for(i = 0 ; i < 8 ; i++)
    {
        OLED_WrDat(OLED_F8x16[c][i + 8]);
    }
}

void OLED_ShowString(uint8_t x , uint8_t y , uint8_t *chr)
{
    unsigned char j = 0;
    while(chr[j] != '\0')
    {
        OLED_ShowChar(x , y , chr[j]);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

void OLED_ShowChinese(uint8_t x , uint8_t y , uint8_t no)
{
    uint8_t t , adder = 0;
    OLED_WrCmd(0xB0 + y);
    OLED_WrCmd(((x & 0xF0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0F));

    for(t = 0 ; t < 16 ; t++)
    {
        OLED_WrDat(GBK[no][t]);
        adder += 1;
    }

    OLED_WrCmd(0xB0 + y + 1);
    OLED_WrCmd(((x & 0xF0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0F));

    for(t = 0 ; t < 16 ; t++)
    {
        OLED_WrDat(GBK[no][t + 16]);
        adder += 1;
    }
}

void OLED_ShowPicture(uint8_t x , uint8_t y , uint8_t sizex , uint8_t sizey , uint8_t BMP[])
{
    uint16_t j = 0;
    uint8_t i , n;

    uint8_t page_len = sizey / 8;

    for(i = 0 ; i < page_len ; i++)
    {
        OLED_WrCmd(0xB0 + y + i);
        OLED_WrCmd(((x & 0xF0) >> 4) | 0x10);
        OLED_WrCmd((x & 0x0F));

        for(n = 0 ; n < sizex ; n++)
        {
            OLED_WrDat(BMP[j++]);
        }
    }
}

