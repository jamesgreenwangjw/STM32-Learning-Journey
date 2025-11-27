/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_RECEIVE_SIZE 200

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t pwm_flag = 0;
volatile uint16_t pwm_vol = 0;
volatile uint32_t pwm_time;
volatile uint16_t adc_buf[2] = {0};
volatile uint32_t adc_get_time = 0;
volatile uint8_t Report_Flag;
volatile uint8_t adc_print_ready = 0;
volatile uint32_t adc_vol_sum = 0;
volatile uint32_t adc_tem_sum = 0;
volatile uint16_t adc_sum_ready = 0;
volatile uint16_t adc_temp_vol = 0;
volatile uint16_t adc_temp_tem = 0;
volatile uint8_t rx_buffer[MAX_RECEIVE_SIZE];
volatile uint32_t warning_limit = 2800;
volatile uint8_t rx_flag = 0;
volatile uint32_t rx_dma_time = 0;
volatile uint32_t light_warning_time = 0;
volatile float adc_tem;
volatile float adc_vol;
volatile uint8_t emergency_flag = 0;
int temp_val = 0;
volatile uint8_t temp_flag = 0;
volatile uint8_t key0_pressed = 0;
volatile uint8_t key1_pressed = 0;
volatile uint8_t processing_buffer[MAX_RECEIVE_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file , char* ptr , int len)
{
	HAL_UART_Transmit(&huart1 , (uint8_t *)ptr , len , HAL_MAX_DELAY);
	return len;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_IWDG_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1 , (uint32_t *)adc_buf , 2);   /* 0 电位器 1 温度 */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1 , (uint8_t *)rx_buffer , MAX_RECEIVE_SIZE);
  HAL_TIM_PWM_Start(&htim3 , TIM_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim6);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(key0_pressed == 1)
	  {
		  emergency_flag = 1;
		  printf("[EMERGENCY] STOPPED!!\r\n");
		  HAL_TIM_PWM_Stop(&htim3 , TIM_CHANNEL_2);
		  key0_pressed = 0;
	  }


	  if(key1_pressed == 1)
	  {
		  printf("System booting......\r\n");
		  HAL_Delay(100);
		  HAL_NVIC_SystemReset();
		  key1_pressed = 0;
	  }


	  if(emergency_flag == 1)
	  {
		  HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_SET);

		  HAL_IWDG_Refresh(&hiwdg);
		  temp_flag = 1;
		  continue;
	  }

	  else if(emergency_flag == 0 && temp_flag == 1)
	  {
		  HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_RESET);
		  temp_flag = 0;
	  }


	  if(HAL_GetTick() - pwm_time >= 1)
	  {
		  if(pwm_vol >= 999)
		  {
			  pwm_flag = 0;
		  }
		  else if(pwm_vol <= 0)
		  {
			  pwm_flag = 1;
		  }

		  if(pwm_flag == 1)
		  {
			  pwm_vol ++;
		  }
		  else if(pwm_flag == 0)
		  {
			  pwm_vol --;
		  }
		  pwm_time = HAL_GetTick();

		  __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_2 , pwm_vol);
	  }


	  if(HAL_GetTick() - adc_get_time >= 10)
	  {
		  if(adc_sum_ready == 10)
		  {
			  adc_temp_vol = adc_vol_sum / 10;
			  adc_temp_tem = adc_tem_sum / 10;
			  adc_vol_sum = 0;
			  adc_tem_sum = 0;
			  adc_sum_ready = 0;
			  adc_print_ready = 1;
		  }

		adc_vol_sum +=  adc_buf[0];
		adc_tem_sum += adc_buf[1];
		adc_sum_ready += 1;

		adc_get_time = HAL_GetTick();
	  }


	  if(Report_Flag == 1)
	  {
		  if(adc_print_ready == 1)
		  {

			  adc_vol = (float)adc_temp_vol * 3.3f / 4096.0f;


			  adc_tem = (1.43f - (float)adc_temp_tem * 3.3f / 4096.0f) / 0.0043f + 25.0f;

			  printf("V : %.2f , T : %.2f\r\n" , adc_vol , adc_tem);
			  adc_print_ready = 0;
		  }
		  Report_Flag = 0;
	  }


	  if(rx_flag == 1)
	  {
		  printf("Debug: %s\r\n", processing_buffer);
		if (sscanf((char *)processing_buffer, "set:%d", &temp_val) == 1)
		{

			  if(temp_val > 0 && temp_val < 4095)
			  {

			      warning_limit = (uint32_t)temp_val;

			      printf("Limit updated to: %lu\r\n", (unsigned long)warning_limit);
			  }
	    }
		memset((void *)rx_buffer, 0, MAX_RECEIVE_SIZE);
		rx_flag = 0;

	  }


	  if(HAL_GetTick() - light_warning_time >= 100) /*5Hz*/
	  {
		  if((int)(adc_vol * 1000) > (int)warning_limit)
		  {
			  HAL_GPIO_TogglePin(GPIOE , GPIO_PIN_5);
		  }
		  light_warning_time = HAL_GetTick();
	  }


	  if(adc_tem > 32.0)
	  {
		  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_SET);
	  }

	  else if(adc_tem > 0.0 && adc_tem <= 32.0)
	  {
		  HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_RESET);
	  }


	  HAL_IWDG_Refresh(&hiwdg);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM6)
	{
		Report_Flag = 1;
	}
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart , uint16_t Size)
{
	if(huart -> Instance == USART1)
	{
		if(Size < MAX_RECEIVE_SIZE)
		{
			memcpy((void *)processing_buffer , (void *)rx_buffer , Size);
			processing_buffer[Size] = '\0';

		}
		rx_flag = 1;
	}

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1 , (uint8_t *)rx_buffer , MAX_RECEIVE_SIZE);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY0_Pin)
	{
		key0_pressed = 1;
	}

	else if(GPIO_Pin == KEY1_Pin)
	{
		key1_pressed = 1;
	}
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
