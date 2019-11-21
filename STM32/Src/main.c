
#include "main.h"
#include "mpu.h"
#include<stdio.h>
#include<string.h>
#include<math.h>



 // CONFIG PROTOTYPES
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART2_UART_Init(void);


int main(void)
{
	uint8_t  buf[4];

  HAL_Init(); /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  SystemClock_Config(); /* Configure the system clock */

  MX_GPIO_Init();
  MX_I2C3_Init();
  MX_USART2_UART_Init();
  MPU6050_Init();

  while (1)
  {
	   MPU6050_Read_Accel();
	   accAngleX = (atan(Ay / sqrt(pow(Ax, 2) + pow(Az, 2))) * 180 / PI);
	   accAngleY = (atan(-1 * Ax / sqrt(pow(Ay, 2) + pow(Az, 2))) * 180 / PI);

	   previousTime = currentTime;
	   currentTime = HAL_GetTick();
	   elapsedTime = (currentTime - previousTime) / 1000;



	    MPU6050_Read_Gyro();
	    gyroAngleX = gyroAngleX + Gx * elapsedTime;
	    gyroAngleY = gyroAngleY + Gy * elapsedTime;
	    yaw =  yaw + Gz * elapsedTime;


	    roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
	    pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

	    int len = sprintf(buf,"%f , %f , %f",roll,yaw,pitch);
	    HAL_UART_Transmit(&huart2,(uint8_t*)buf,len,0x0002);
	    HAL_Delay(1000);
	    /*char buffer[]= "\r\n\0\0\0";
	    if (roll>0)
	    {

	    	strcpy(buffer, "\r\n ROLL= - ");
	    	HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	    	ftoa(roll, buf, 2);
	    	HAL_UART_Transmit(&huart2,buf, 10, 100);

	    }
	    else
	    {
	    	strcpy(buffer, "\r\n ROLL= ");
	    	HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	    	ftoa(roll, buf, 2);
	    	HAL_UART_Transmit(&huart2,buf, 10, 100);*

	    }*/
	    /*strcpy(buffer, "\r\n PITCH= \0\0\0");
	    HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	    ftoa(pitch, buf, 2);
	    HAL_UART_Transmit(&huart2,buf, 10, 100);
	    strcpy(buffer, "\r\n YAW= \0\0\0");
	    HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	    ftoa(yaw, buf, 2);
	     HAL_UART_Transmit(&huart2,buf, 10, 100);*/


 /* USER CODE END WHILE */
/*		  char buffer[]= "\r\nAx=";
		   HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
		  ftoa(Ax, buf, 4);
		   HAL_UART_Transmit(&huart2,buf, 10, 100);
		   strcpy(buffer, "\r\nAy=");
		  HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
		  ftoa(Ay, buf, 4);
		  HAL_UART_Transmit(&huart2,buf, 10, 100);
		  strcpy(buffer, "\r\nAz=");
		  HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
		  ftoa(Az, buf, 4);
		  HAL_UART_Transmit(&huart2,buf, 10, 100);

		  strcpy(buffer, "\r\nGx=");
		 HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
		 ftoa(Gx, buf, 4);
	      HAL_UART_Transmit(&huart2,buf, 10, 100);
	   	  strcpy(buffer, "\r\nGy=");
	  	  HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	  	  ftoa(Gy, buf, 4);
		  HAL_UART_Transmit(&huart2,buf, 10, 100);
		  strcpy(buffer, "\r\nGz=");
	      HAL_UART_Transmit(&huart2,(uint8_t *)buffer, 10, 100);
	      ftoa(Gz, buf, 4);
	      HAL_UART_Transmit(&huart2,buf, 10, 100);*/






  }

}




               //CONFIG FUNCTION
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
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
}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

