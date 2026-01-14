/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"oled.h"
#include"led.h"
#include"oledfont.h"
#include"main.h"
#include"gpio.h"
#include"motor.h"
#include"iwdg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
typedef enum {
    STATE_IDLE = 0, // 待机/停止
    STATE_RUN,      // 运行
    STATE_ESTOP     // 急停锁死
} CarState_t;
volatile CarState_t g_CarState = STATE_IDLE;
/* USER CODE END Variables */
/* Definitions for Task_Drive */
osThreadId_t Task_DriveHandle;
const osThreadAttr_t Task_Drive_attributes = {
  .name = "Task_Drive",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_Display */
osThreadId_t Task_DisplayHandle;
const osThreadAttr_t Task_Display_attributes = {
  .name = "Task_Display",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Logic */
osThreadId_t Task_LogicHandle;
const osThreadAttr_t Task_Logic_attributes = {
  .name = "Task_Logic",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for Task_Dog */
osThreadId_t Task_DogHandle;
const osThreadAttr_t Task_Dog_attributes = {
  .name = "Task_Dog",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BinarySem_Start */
osSemaphoreId_t BinarySem_StartHandle;
const osSemaphoreAttr_t BinarySem_Start_attributes = {
  .name = "BinarySem_Start"
};
/* Definitions for BinarySem_Stop */
osSemaphoreId_t BinarySem_StopHandle;
const osSemaphoreAttr_t BinarySem_Stop_attributes = {
  .name = "BinarySem_Stop"
};
/* Definitions for BinarySem_EStop */
osSemaphoreId_t BinarySem_EStopHandle;
const osSemaphoreAttr_t BinarySem_EStop_attributes = {
  .name = "BinarySem_EStop"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTaskDrive(void *argument);
void StartTaskDisplay(void *argument);
void StartTaskLogic(void *argument);
void StartTaskDog(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem_Start */
  BinarySem_StartHandle = osSemaphoreNew(1, 1, &BinarySem_Start_attributes);

  /* creation of BinarySem_Stop */
  BinarySem_StopHandle = osSemaphoreNew(1, 1, &BinarySem_Stop_attributes);

  /* creation of BinarySem_EStop */
  BinarySem_EStopHandle = osSemaphoreNew(1, 1, &BinarySem_EStop_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Drive */
  Task_DriveHandle = osThreadNew(StartTaskDrive, NULL, &Task_Drive_attributes);

  /* creation of Task_Display */
  Task_DisplayHandle = osThreadNew(StartTaskDisplay, NULL, &Task_Display_attributes);

  /* creation of Task_Logic */
  Task_LogicHandle = osThreadNew(StartTaskLogic, NULL, &Task_Logic_attributes);

  /* creation of Task_Dog */
  Task_DogHandle = osThreadNew(StartTaskDog, NULL, &Task_Dog_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTaskDrive */
/**
  * @brief  Function implementing the Task_Drive thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskDrive */
void StartTaskDrive(void *argument)
{
  /* USER CODE BEGIN StartTaskDrive */
	Motor_Init();
  /* Infinite loop */
  for(;;)
  {
	  switch (g_CarState)
	      {
	          case STATE_RUN:
	              // 全速前进最大速度 (1000, 1000)
	              // 这是我留下的微调左右轮速度的地方，如果跑偏可以调整,比如 (1000, 950)
	              Motor_SetSpeed(1000 , 1000);
	              break;

	          case STATE_IDLE:
	              Motor_Stop();
	              break;

	          case STATE_ESTOP:
	              Motor_Stop();
	              break;

	          default:
	              Motor_Stop();
	              break;
	      }

	      osDelay(10);
  }
  /* USER CODE END StartTaskDrive */
}

/* USER CODE BEGIN Header_StartTaskDisplay */
/**
* @brief Function implementing the Task_Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDisplay */
void StartTaskDisplay(void *argument)
{
  /* USER CODE BEGIN StartTaskDisplay */
	OLED_Init();
	OLED_Clear();

	  // 这里我设置的静态显示队名
	OLED_ShowString(0 , 0 , (uint8_t*)"[ TEAM 06 ]");
  /* Infinite loop */
	  for(;;)
	  {
		  // 这里我设置的根据状态显示不同内容和灯光
		      if (g_CarState == STATE_RUN)
		      {
		          OLED_ShowString(0 , 3 , (uint8_t*)"Status: RUN   ");
		          // 跑的时候绿灯
		          LED_Set(LED_GREEN);
		      }
		      else if (g_CarState == STATE_IDLE)
		      {
		          OLED_ShowString(0 , 3 , (uint8_t*)"Status: STOP  ");
		          // 停的时候红灯
		          LED_Set(LED_RED);
		      }

		      // 这里可以额外添加心跳闪烁 (利用空余的 RGB 组合颜色，比如黄色闪烁),我懒得弄了
		      // 为了不覆盖状态灯，也可以不做闪烁，或者只在STOP时我额外添加一个呼吸灯也行

		      osDelay(200);
	  }
  /* USER CODE END StartTaskDisplay */
}

/* USER CODE BEGIN Header_StartTaskLogic */
/**
* @brief Function implementing the Task_Logic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLogic */
void StartTaskLogic(void *argument)
{
  /* USER CODE BEGIN StartTaskLogic */
  /* Infinite loop */
  for(;;)
  {
	  if (osSemaphoreAcquire(BinarySem_EStopHandle , 0) == osOK)
	      {
	          g_CarState = STATE_ESTOP;

	          Motor_Stop();
	          LED_Set(LED_BLUE); // 蓝灯警示
	          OLED_Clear();
	          OLED_ShowString(0 , 0 , (uint8_t*)"E-STOP");
	          OLED_ShowString(0 , 2 , (uint8_t*)"PRESS RESET");

	          while(1)
	          {
	              HAL_GPIO_TogglePin(B_GPIO_Port , B_Pin); // 蓝灯闪
	              HAL_IWDG_Refresh(&hiwdg);
	              HAL_Delay(100);
	          }
	      }

	      // 检查停止
	      if (osSemaphoreAcquire(BinarySem_StopHandle , 0) == osOK)
	      {
	          g_CarState = STATE_IDLE;
	      }

	      // 检查启动
	      if (osSemaphoreAcquire(BinarySem_StartHandle , 0) == osOK)
	      {
	          if (g_CarState != STATE_ESTOP)
	          {
	              g_CarState = STATE_RUN;
	          }
	      }

	      osDelay(20);
  }
  /* USER CODE END StartTaskLogic */
}

/* USER CODE BEGIN Header_StartTaskDog */
/**
* @brief Function implementing the Task_Dog thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDog */
void StartTaskDog(void *argument)
{
  /* USER CODE BEGIN StartTaskDog */
  /* Infinite loop */
  for(;;)
  {
	  HAL_IWDG_Refresh(&hiwdg);
	  osDelay(1000); // 这里我设置的1s喂一次,我mx里配置的是最大4s不喂狗就重启,所以时间还是很充裕的,但是会导致报错不及时,所以等17号实战的时候我需要调低!
  }
  /* USER CODE END StartTaskDog */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

