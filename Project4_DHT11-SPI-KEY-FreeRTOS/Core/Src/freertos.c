/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include"string.h"
#include"tim.h"
#include"iwdg.h"
#include"delayus.h"
#include"DHT11.h"
#include"display.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LEN 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern volatile uint16_t adc_buf[MAX_LEN];
typedef struct
{
	float tem;
	uint16_t vol;
}sensordata;
sensordata sen1;
volatile uint8_t display_buf[5] = {0};
/* USER CODE END Variables */
/* Definitions for TaskSensor */
osThreadId_t TaskSensorHandle;
const osThreadAttr_t TaskSensor_attributes = {
  .name = "TaskSensor",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskSystem */
osThreadId_t TaskSystemHandle;
const osThreadAttr_t TaskSystem_attributes = {
  .name = "TaskSystem",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for TaskDisplay */
osThreadId_t TaskDisplayHandle;
const osThreadAttr_t TaskDisplay_attributes = {
  .name = "TaskDisplay",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskLed */
osThreadId_t TaskLedHandle;
const osThreadAttr_t TaskLed_attributes = {
  .name = "TaskLed",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskDog */
osThreadId_t TaskDogHandle;
const osThreadAttr_t TaskDog_attributes = {
  .name = "TaskDog",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for sensordataMutex */
osMutexId_t sensordataMutexHandle;
const osMutexAttr_t sensordataMutex_attributes = {
  .name = "sensordataMutex"
};
/* Definitions for SemRestart */
osSemaphoreId_t SemRestartHandle;
const osSemaphoreAttr_t SemRestart_attributes = {
  .name = "SemRestart"
};
/* Definitions for SemStop */
osSemaphoreId_t SemStopHandle;
const osSemaphoreAttr_t SemStop_attributes = {
  .name = "SemStop"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTaskSensor(void *argument);
void StartTaskSystem(void *argument);
void StartTaskDisplay(void *argument);
void StartTaskLed(void *argument);
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
  /* Create the mutex(es) */
  /* creation of sensordataMutex */
  sensordataMutexHandle = osMutexNew(&sensordataMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of SemRestart */
  SemRestartHandle = osSemaphoreNew(1, 1, &SemRestart_attributes);

  /* creation of SemStop */
  SemStopHandle = osSemaphoreNew(1, 0, &SemStop_attributes);

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
  /* creation of TaskSensor */
  TaskSensorHandle = osThreadNew(StartTaskSensor, NULL, &TaskSensor_attributes);

  /* creation of TaskSystem */
  TaskSystemHandle = osThreadNew(StartTaskSystem, NULL, &TaskSystem_attributes);

  /* creation of TaskDisplay */
  TaskDisplayHandle = osThreadNew(StartTaskDisplay, NULL, &TaskDisplay_attributes);

  /* creation of TaskLed */
  TaskLedHandle = osThreadNew(StartTaskLed, NULL, &TaskLed_attributes);

  /* creation of TaskDog */
  TaskDogHandle = osThreadNew(StartTaskDog, NULL, &TaskDog_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTaskSensor */
/**
  * @brief  Function implementing the TaskSensor thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskSensor */
void StartTaskSensor(void *argument)
{
  /* USER CODE BEGIN StartTaskSensor */
  /* Infinite loop */
	uint16_t temp_buf[MAX_LEN];
	uint16_t temp_vol = 0;
	uint32_t sum_tem;
	uint32_t sum_vol;
	uint32_t temp_tem;
	int i = 0;
  for(;;)
  {
	  sum_vol = 0;
	  sum_tem = 0;
	  memcpy(temp_buf , (void *)adc_buf , sizeof(temp_buf));
	  for(i = 0 ; i < MAX_LEN ; i++)
	  {

		  if(i % 2 == 0)
		  {
			  sum_vol += temp_buf[i];
		  }
		  else
		  {
			  sum_tem += temp_buf[i];
		  }
	  }
	  temp_vol = sum_vol / (MAX_LEN / 2);
	  temp_tem = sum_tem / (MAX_LEN / 2);
	  float voltage = (float)temp_tem * 3.3f / 4096.0f;
	  float cpu_temp_calc = (1.43f - voltage) / 0.0043f + 25.0f;

	  if(osMutexAcquire(sensordataMutexHandle , osWaitForever) == osOK)
	  {
		  sen1.vol = temp_vol;
		  sen1.tem = cpu_temp_calc;
		  osMutexRelease(sensordataMutexHandle);
	  }

    osDelay(100);
	//debug 我DHT11一直连接有问题,调试代码放在这里了
	/*
	#define LED_DEBUG_ERR_CHECK(x)   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, x) // PC0
	#define LED_DEBUG_ERR_TIMEOUT(x) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, x) // PC1
	#define LED_DEBUG_ERR_SUM(x)     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, x) // PC2
	#define LED_DEBUG_RUN_TOGGLE()   HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3)   // PC3


	  uint16_t temp_vols[MAX_LEN];
	  uint16_t temp_vol = 0;
	  uint32_t sum;
	  int i = 0;

	  uint8_t dht11_buf[5] = {0, 0, 0, 0, 0};
	  float temp_tem = 0;
	  float temp_moi = 0;
	  uint8_t res = 0;

	  for(;;)
	  {
		  LED_DEBUG_ERR_CHECK(GPIO_PIN_SET);
		  LED_DEBUG_ERR_TIMEOUT(GPIO_PIN_SET);
		  LED_DEBUG_ERR_SUM(GPIO_PIN_SET);
	      LED_DEBUG_RUN_TOGGLE();

	      res = ReadByte((uint8_t *)dht11_buf);

	      if(res == 0)
	      {

	          LED_DEBUG_ERR_CHECK(GPIO_PIN_SET);
	          LED_DEBUG_ERR_TIMEOUT(GPIO_PIN_SET);
	          LED_DEBUG_ERR_SUM(GPIO_PIN_SET);

	          temp_moi = dht11_buf[0] + (dht11_buf[1] / 10.0f);
	          temp_tem = dht11_buf[2] + (dht11_buf[3] / 10.0f);

	          memcpy((void*)display_buf , (void*)dht11_buf , sizeof(dht11_buf));
	      }
	      else
	      {
	    	  temp_moi = dht11_buf[0] + (dht11_buf[1] / 10.0f);
	    	  temp_tem = dht11_buf[2] + (dht11_buf[3] / 10.0f);

	    	  memcpy((void*)display_buf , (void*)dht11_buf , sizeof(dht11_buf));
	          if(res == 1) {

	              LED_DEBUG_ERR_CHECK(GPIO_PIN_RESET);
	          }
	          else if(res == 2) {

	              LED_DEBUG_ERR_SUM(GPIO_PIN_RESET);
	          }
	      }


	      sum = 0;
	      memcpy(temp_vols , (void *)adc_vol , sizeof(temp_vols));
	      for(i = 0 ; i < MAX_LEN ; i++)
	      {
	          sum += temp_vols[i];
	      }
	      temp_vol = sum / MAX_LEN;

	      if(osMutexAcquire(sensordataMutexHandle , osWaitForever) == osOK)
	      {
	          sen1.vol = temp_vol;
	          sen1.moi = temp_moi;
	          sen1.tem = temp_tem;
	          osMutexRelease(sensordataMutexHandle);
	      }

	      osDelay(2000);
	      */
	  }

  /* USER CODE END StartTaskSensor */
}

/* USER CODE BEGIN Header_StartTaskSystem */
/**
* @brief Function implementing the TaskSystem thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSystem */
void StartTaskSystem(void *argument)
{
  /* USER CODE BEGIN StartTaskSystem */
	osSemaphoreAcquire(SemStopHandle, 0);
  /* Infinite loop */
  for(;;)
  {
	  if(osSemaphoreAcquire(SemStopHandle , osWaitForever) == osOK)
	  {
		  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_RESET);
		  osThreadSuspend(TaskLedHandle);
		  osSemaphoreAcquire(SemRestartHandle, 0);
		  while(1)
		  {
			  if(osSemaphoreAcquire(SemRestartHandle , 0) == osOK)
			  {
			  	  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_SET);
			  	  HAL_NVIC_SystemReset();
			  }
			  HAL_GPIO_TogglePin(GPIOC , LED5_Pin);
			  HAL_IWDG_Refresh(&hiwdg);
			  osDelay(200);
		  }
	  }
    osDelay(10);
  }
  /* USER CODE END StartTaskSystem */
}

/* USER CODE BEGIN Header_StartTaskDisplay */
/**
* @brief Function implementing the TaskDisplay thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDisplay */
void StartTaskDisplay(void *argument)
{
  /* USER CODE BEGIN StartTaskDisplay */
  /* Infinite loop */
	uint8_t DataBuffer[8] = {0};
	float temp_val = 0.0f;
	uint16_t vol_val = 0;
  for(;;)
  {
	   if(osMutexAcquire(sensordataMutexHandle , 10) == osOK)
	   {
	        vol_val = sen1.vol;
	        temp_val = sen1.tem;
	        osMutexRelease(sensordataMutexHandle);
	   }

	   uint16_t temp_int = (uint16_t)(temp_val * 10);
       DataBuffer[0] = (temp_int / 100) % 10;
       DataBuffer[1] = (temp_int / 10) % 10;
       DataBuffer[2] = temp_int % 10;
       DataBuffer[3] = 0;

       uint16_t voltage_disp = (vol_val * 330) / 4096;

	   DataBuffer[4] = (voltage_disp / 100) % 10;
	   DataBuffer[5] = (voltage_disp / 10) % 10;
	   DataBuffer[6] = voltage_disp % 10;
	   DataBuffer[7] = 0;

	  Display_Scan(DataBuffer);

    osDelay(1);
  }
  /* USER CODE END StartTaskDisplay */
}

/* USER CODE BEGIN Header_StartTaskLed */
/**
* @brief Function implementing the TaskLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLed */
void StartTaskLed(void *argument)
{
  /* USER CODE BEGIN StartTaskLed */
  /* Infinite loop */
	uint32_t pwm_adc_vol = 0;
	uint32_t pwm_vol = 0;
	int flag = 0; // 0up 1down
	static uint8_t flow_cnt = 0;
    static uint8_t flow_step = 0;
	static uint8_t alarm_cnt = 0;
	const uint16_t ALARM_THRESHOLD = 3475;
  for(;;)
  {
	  if(osMutexAcquire(sensordataMutexHandle , osWaitForever) == osOK)
	  {
		  pwm_adc_vol = sen1.vol;
		  osMutexRelease(sensordataMutexHandle);
	  }

	  if (pwm_adc_vol > ALARM_THRESHOLD)
	  {
		  alarm_cnt ++;
		  if(alarm_cnt >= 10)
		  {
			  alarm_cnt = 0;
			  GPIOC->ODR ^= GPIO_PIN_0;
		  }
	  }
	  else
	  {
		  GPIOC->BSRR = GPIO_PIN_0;
		  alarm_cnt = 0;
	  }

	  flow_cnt ++;
	  if(flow_cnt >= 20)
	  {
		  flow_cnt = 0;

		  GPIOC->BSRR = (GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
		  switch(flow_step)
		  {
			  case 0: GPIOC->BRR = GPIO_PIN_2; break;
			  case 1: GPIOC->BRR = GPIO_PIN_3; break;
			  case 2: GPIOC->BRR = GPIO_PIN_4; break;
		  }

		  flow_step ++;
		  if(flow_step > 2) flow_step = 0;
	  }


	  if (pwm_adc_vol < 50) {
	      pwm_adc_vol = 0;
	  }
	  if(pwm_vol >= 999)
	  {
		  flag = 1;
	  }
	  else if(pwm_vol <= 0)
	  {
		  flag = 0;
	  }
	  if(flag == 0)
	  {
		  pwm_vol += 20;
	  }
	  else if(flag == 1)
	  {
		  pwm_vol -= 20;
	  }
	  uint32_t mapped_adc = (pwm_adc_vol * 1000) / 4096;
	  if(mapped_adc <= 200)
	  {
		  mapped_adc = 0;
	  }
	  __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_2 , mapped_adc);
	  __HAL_TIM_SET_COMPARE(&htim8 , TIM_CHANNEL_1 , pwm_vol);




    osDelay(10);
  }
  /* USER CODE END StartTaskLed */
}

/* USER CODE BEGIN Header_StartTaskDog */
/**
* @brief Function implementing the TaskDog thread.
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
    osDelay(2000);
  }
  /* USER CODE END StartTaskDog */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

