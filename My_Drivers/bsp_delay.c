#include "bsp_delay.h"

void BSP_Delay_Init(void)
{
    HAL_TIM_Base_Start(&htim6);
}
void delay_us(uint32_t us)
{
    uint16_t start_val = __HAL_TIM_GET_COUNTER(&htim6);

    __HAL_TIM_SET_COUNTER(&htim6, 0);
    while (__HAL_TIM_GET_COUNTER(&htim6) < us);
}

void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}