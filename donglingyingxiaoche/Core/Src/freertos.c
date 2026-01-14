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
void StartTask03(void *argument);
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
  Task_LogicHandle = osThreadNew(StartTask03, NULL, &Task_Logic_attributes);

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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskDisplay */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the Task_Logic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
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
    osDelay(1);
  }
  /* USER CODE END StartTaskDog */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

