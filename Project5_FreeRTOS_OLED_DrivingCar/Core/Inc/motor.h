/*
 * motor.h
 *
 *  Created on: Jan 14, 2026
 *      Author: wangjunwei
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include"main.h"
#include"gpio.h"

/*
 * 电机最大PWM值，对应CubeMX里TIM1的Counter Period (999)
 * 范围: -1000 到 +1000
 */
#define MOTOR_MAX_PWM 1000

void Motor_Init(void);
void Motor_Stop(void); // 急停
void Motor_SetSpeed(int16_t speed_left , int16_t speed_right); // 速度控制

#endif /* INC_MOTOR_H_ */
