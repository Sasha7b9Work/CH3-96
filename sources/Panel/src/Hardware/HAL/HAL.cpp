// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Hardware/FPGA.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


#ifdef PANEL
    static PCD_HandleTypeDef hPCD;
    void *HAL_PCD::handle = &hPCD;
#endif


static void SystemClock_Config(void);


void HAL::Init()
{
    HAL_Init();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_11 | GPIO_PIN_12,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF10_OTG_FS
    };

    HAL_GPIO_Init(GPIOA, &isGPIO);

    isGPIO.Pin = GPIO_PIN_9;
    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &isGPIO);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

    SystemClock_Config();

#ifdef PANEL
    HAL_FSMC::Init();
#endif

    HAL_TIM::Init();
}


void HAL::DeInit()
{
    HAL_DeInit();
}


static void SystemClock_Config()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG

    (PWR_REGULATOR_VOLTAGE_SCALE2);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }
}


void HAL::ERROR_HANDLER()
{
    *((int *)((void *)0)) = 0;
}
