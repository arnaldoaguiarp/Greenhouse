/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
ADC_HandleTypeDef hadc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

int ler_AD1 (int numero);

//uint16_t ajustHumidity(utint16_t temperaturaHardw);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void int_to_string(int value, char* str, int size) {
    for(int i = size-1; i >= 0; i--) {
        str[i] ='0' + value%10;
        value /= 10;
    }
}

void strcopy(char* destination, char* source, int begin, int end) {
    for(int i = begin; i <= end; i++) {
        destination[i] = source[i-begin];
    }
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
  MX_ADC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */



char msg[50];

int valueHumitidy = 0;
int valueTemperature = 0;

char valueStrH[4];
char valueStrT[4];

HAL_StatusTypeDef x ;
HAL_StatusTypeDef y ;
uint8_t buf1[100];
memset(buf1, 0, 100);


//ESP answers OK
	x = HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n", 4, 2000);
	y = HAL_UART_Receive(&huart1, buf1, 100, 3000);
	//HAL_Delay(300);
  //Sets the Wi-Fi mode as station mode (client)
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CWMODE=1\r\n", 13, 2000);
	HAL_UART_Receive(&huart1, buf1, 100, 3000);
	HAL_Delay(300);
  //Name and password of the Wi-Fi
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CWJAP=\"Arnaldo\",\"10102142\"\r\n", 50, 2000);
	HAL_UART_Receive(&huart1, buf1, 100, 10000);
	HAL_Delay(300);
  //Sets the communication mode as single mode
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPMUX=0\r\n", 13, 2000);
	HAL_UART_Receive(&huart1, buf1, 100, 2000);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  valueHumitidy = 0;
	  valueTemperature = 0;

	  //Message1 to thinkspeak
	    	  strcpy(msg,"GET /update?api_key=B4XQN73HCTMIZTMM&field1=0000\r\n");
	  //Read the level of humidity from the sensor
	          HAL_Delay(1000);
	          valueHumitidy = ler_AD1((int) 1);
	          HAL_Delay(1000);
	          valueHumitidy = ler_AD1((int) 1);
	          HAL_Delay(1000);
	          valueHumitidy=(valueHumitidy*100)/4095;
	          HAL_Delay(1000);
	  //Add this valueHumitidy to the message
	          int_to_string(valueHumitidy, valueStrH, 4);
	          strcopy(msg, valueStrH, 44, 47);
	  //Starts the communication
	          HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 70, 2000);
	          HAL_UART_Receive(&huart1, buf1, 100, 5000);
	          HAL_Delay(1000);
	  //Inform the number of bytes will be sended
	          HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPSEND=50\r\n", 15, 2000);
	          HAL_UART_Receive(&huart1, buf1, 100, 5000);
	          HAL_Delay(500);
	  //Transmit the message to thinkspeak through the ESP
	          HAL_UART_Transmit(&huart1, msg, 50, 2000);
	          HAL_UART_Receive(&huart1, buf1, 100, 5000);
	          HAL_Delay(20000);
	  //Message2 to thinkspeak
			  strcpy(msg,"GET /update?api_key=B4XQN73HCTMIZTMM&field2=0000\r\n");
	  //Read the level of temperature from the sensor
			  HAL_Delay(1000);
			  valueTemperature = ler_AD1((int) 0);
			  HAL_Delay(1000);
			  valueTemperature = ler_AD1((int) 0);
			  HAL_Delay(1000);
			  valueTemperature = ((valueTemperature*205)/4095);
			  HAL_Delay(1000);
	  //Add this value to the message
			  int_to_string(valueTemperature, valueStrT, 4);
			  strcopy(msg, valueStrT, 44, 47);
	  //Starts the communication
			  HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 70, 2000);
			  HAL_UART_Receive(&huart1, buf1, 100, 5000);
			  HAL_Delay(1000);
	  //Inform the number of bytes will be sended
			  HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPSEND=50\r\n", 15, 2000);
			  HAL_UART_Receive(&huart1, buf1, 100, 5000);
			  HAL_Delay(500);
	  //Transmit the message to thinkspeak through the ESP
			  HAL_UART_Transmit(&huart1, msg, 50, 2000);
			  HAL_UART_Receive(&huart1, buf1, 100, 5000);
			  HAL_Delay(20000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

int ler_AD1 (int numero){
	ADC_ChannelConfTypeDef chConfig = {0};
	switch(numero){
	case 0:
			chConfig.Channel = ADC_CHANNEL_0;
			break;
	case 1:
			chConfig.Channel = ADC_CHANNEL_1;
			break;

	}

	HAL_ADC_ConfigChannel(&hadc, &chConfig);
	HAL_Delay(500);
	HAL_ADC_Start(&hadc);
	HAL_Delay(500);
	int v = HAL_ADC_GetValue(&hadc);
	HAL_Delay(500);
	HAL_ADC_Stop(&hadc);
		chConfig.Rank = ADC_RANK_NONE;
		HAL_ADC_ConfigChannel(&hadc, &chConfig);
	return v;
}


/*uint16_t ajustHumidity(utint16_t umidadeHardw){
	return (umidadeHardw/4065) * 100;

}*/


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
