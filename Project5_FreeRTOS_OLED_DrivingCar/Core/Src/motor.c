
/*
 * motor.c
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */

#include"motor.h"
#include"tim.h"
#include"gpio.h"
#define MOTOR_MAX_PWM 1000
//左右轮的函数均设置为static,也就是内部函数
// 设置左轮
static void Motor_Set_Left(int16_t speed)
{
    // 1. 限幅
    if (speed > MOTOR_MAX_PWM) speed = MOTOR_MAX_PWM;
    if (speed < -MOTOR_MAX_PWM) speed = -MOTOR_MAX_PWM;
    // 2.  方向控制
    if (speed > 0)
    {
        // 正转
        HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_RESET);
    }
    else if (speed < 0)
    {
        // 反转
        HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_SET);
        speed = -speed;
    }
    else
    {
        // 刹车
        HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_RESET);
    }

    // 3. 设置 PWM
    __HAL_TIM_SET_COMPARE(&htim1 , TIM_CHANNEL_1 , speed);
}

//设置右轮
static void Motor_Set_Right(int16_t speed)
{
    if (speed > MOTOR_MAX_PWM) speed = MOTOR_MAX_PWM;
    if (speed < -MOTOR_MAX_PWM) speed = -MOTOR_MAX_PWM;

    if (speed > 0)
    {
        // 正转
        HAL_GPIO_WritePin(BIN1_GPIO_Port , BIN1_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port , BIN2_Pin , GPIO_PIN_RESET);
    }
    else if (speed < 0)
    {
        // 反转
        HAL_GPIO_WritePin(BIN1_GPIO_Port , BIN1_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port , BIN2_Pin , GPIO_PIN_SET);
        speed = -speed;
    }
    else
    {
        // 刹车
        HAL_GPIO_WritePin(BIN1_GPIO_Port , BIN1_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port , BIN2_Pin , GPIO_PIN_RESET);
    }

    // 3. 设置 PWM
    __HAL_TIM_SET_COMPARE(&htim1 , TIM_CHANNEL_2 , speed);
}

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1 , TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1 , TIM_CHANNEL_2);
    Motor_Stop();
}

void Motor_Stop(void)
{
    Motor_Set_Left(0);
    Motor_Set_Right(0);
}

void Motor_SetSpeed(int16_t speed_left , int16_t speed_right)
{
    Motor_Set_Left(speed_left);
    Motor_Set_Right(speed_right);
}
