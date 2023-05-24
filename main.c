#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "lcd_txt.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);

ADC_HandleTypeDef hadc1;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();

	lcd_init();

    float t1 = 0, t2 = 0, t = 0, v = 0;
    char vel[10];

    float latitude = 0, longitude = 0;
    char crd[10];

    uint16_t adc = 0;
    int db = 0;
    char dB[10];

    while (1)
    {
    	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) { t1 = HAL_GetTick(); }
	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) { t2 = HAL_GetTick(); }

	  	HAL_ADC_Start(&hadc1);
		adc = HAL_ADC_GetValue(&hadc1);
		db = (adc + 83.2073) / 11.003;
	  	if(db < 0)
	  	{
	  		db = 0;
	 	}
	  	else if(db > 150)
	  	{
	  		db = 150;
	 	}
	  	sprintf(dB, "dB:%d", db);
	  	HAL_ADC_Stop(&hadc1);

	  	int rand1 = rand()%2;
	 	int rand2 = rand()%2;
	 	if(rand1 == 0)
	 	{
	  		latitude = ((float)rand()/(float)(RAND_MAX)) * 90.0f;
	  	}
	  	else
	  	{
	  		latitude = - ((float)rand()/(float)(RAND_MAX)) * 90.0f;
		}

		if(rand2 == 0)
	  	{
	  		longitude = ((float)rand()/(float)(RAND_MAX)) * 180.0f;
	  	}
	  	else
		{
	  		longitude = - ((float)rand()/(float)(RAND_MAX)) * 180.0f;
	 	}

	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
	  	{
	  		t = t2 - t1;
	  		t = t / 1000 / 3600;
	  		v = 0.02 / t;

	  		if(v < 0)
	  		{
	  			v = v * (-1);
	  			sprintf(vel, "Vel:%.0fkm/h Dir:L", v);
	  		}
	  		else
	  		{
	  			sprintf(vel, "Vel:%.0fkm/h Dir:R", v);
	  		}

	  		lcd_puts(0, 0, (int8_t*) vel);
	  		lcd_puts(1, 0, (int8_t*) dB);

	  		sprintf(crd, "Crd:(%.2f,%.2f)", latitude, longitude);
	  		lcd_puts(3, 0, (int8_t*) crd);

	  		if(v > 120 || db > 120)
	  		{
	  			if(v > 120 && db > 120)
	  			{
	  				lcd_puts(2, 0, "H.Speed - A.Noise");
	  			}
	  			else if(db > 120)
	  			{
	  				lcd_puts(2, 0, "Annoying Noise");
	  			}
	  			else if(v > 120)
	  			{
	  				lcd_puts(2, 0, "High Speed");
	  			}

	  			for(int i=1; i<=10; i++)
	  			{
	  				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	  				HAL_Delay(100);
	  				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
	  				HAL_Delay(100);
	  			}
	  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	  		}
	  	}
    }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
