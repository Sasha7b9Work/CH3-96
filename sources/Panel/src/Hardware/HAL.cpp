#include "HAL.h"
#include "stm32f4xx_hal.h"


static void SystemClock_Config(void);

static SPI_HandleTypeDef handleSPI1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::Init()
{
    HAL_Init();
    SystemClock_Config();
    SPI1_::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemClock_Config(void)
{
 
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    
    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      ERROR_HANDLER();
    }
    
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::SPI1_::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();

    GPIO_InitTypeDef  isGPIO;

    isGPIO.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_PULLUP;
    isGPIO.Speed = GPIO_SPEED_LOW;
    isGPIO.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    handleSPI1.Instance = SPI1;
    handleSPI1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    handleSPI1.Init.Direction = SPI_DIRECTION_1LINE;
    handleSPI1.Init.CLKPhase = SPI_PHASE_2EDGE;
    handleSPI1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    handleSPI1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handleSPI1.Init.CRCPolynomial = 7;
    handleSPI1.Init.DataSize = SPI_DATASIZE_8BIT;
    handleSPI1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handleSPI1.Init.NSS = SPI_NSS_SOFT;
    handleSPI1.Init.TIMode = SPI_TIMODE_DISABLE;
    handleSPI1.Init.Mode = SPI_MODE_MASTER;

    if (HAL_SPI_Init(&handleSPI1) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    isGPIO.Pin = GPIO_PIN_5;                // Óïðàâëåíèå D/C# äèñïëåÿ
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    isGPIO.Pin = GPIO_PIN_4;                // Óïðàâëåíèå CS# äèñïëåÿ
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    isGPIO.Pin = GPIO_PIN_1;                // Óïðàâëåíèå RES# äèñïëåÿ
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::SPI1_::Send(uint8 *data, uint16 num)
{
    HAL_SPI_Transmit(&handleSPI1, (uint8 *)data, num, 1000);
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ERROR_HANDLER(void)
{
    *((int*)((void*)0)) = 0;
}
