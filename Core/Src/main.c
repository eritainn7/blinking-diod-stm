/* main.c - мигание LED1 (PB0) на NUCLEO-F767ZI */

#include "main.h"
#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "gpio.h"

/* Прототипы функций */
void SystemClock_Config(void);


/**
 * @brief  Главная функция
 */
int main(void)
{
    /* Инициализация HAL библиотеки */
    HAL_Init();
    
    /* Настройка системного тактирования (216 МГц) */
    SystemClock_Config();
    
    /* Инициализация GPIO */
    MX_GPIO_Init();
    
    /* Основной цикл - мигание LED1 (PB0) */
    while (1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // LED1 включен
        HAL_Delay(500);                                         // ждём 500 мс
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // LED1 выключен
        HAL_Delay(500);                                         // ждём 500 мс
    }
}

/**
 * @brief  Настройка системного тактирования
 * @note   Настраивает PLL на 216 МГц от внешнего кварца 8 МГц
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Настройка генераторов */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    // Включаем внешний кварц 8 МГц
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                // Включаем PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        // PLL берёт сигнал с HSE
    RCC_OscInitStruct.PLL.PLLM = 8;                             // HSE / 8 = 8/8 = 1
    RCC_OscInitStruct.PLL.PLLN = 432;                           // 1 * 432 = 432
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                 // 432 / 2 = 216 МГц (SYSCLK)
    RCC_OscInitStruct.PLL.PLLQ = 9;                             // 432 / 9 = 48 МГц (USB)
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Настройка шин (AHB, APB1, APB2) */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // SYSCLK = 216 МГц
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // HCLK = SYSCLK / 1 = 216 МГц
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           // APB1 = HCLK / 4 = 54 МГц
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           // APB2 = HCLK / 2 = 108 МГц

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  Обработчик ошибок
 */
void Error_Handler(void)
{
    /* Бесконечный цикл - можно добавить мигание светодиодом ошибки */
    while (1)
    {
        // Мигаем быстро, сигнализируя об ошибке
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        for(volatile int i = 0; i < 100000; i++);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        for(volatile int i = 0; i < 100000; i++);
    }
}