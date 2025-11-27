/**
 * @file   main.c
 * @author Pratik Dhulubulu
 * @brief  This file initializes the system clock and demonstrates basic 
 *         MCU bring-up flow using the RCC driver.
 */
#include "stm32f446xx.h"
#include "rcc_driver.h"

/**
 * @section Private Data Definations.
 */

/**
 * @brief   System clock configuration.
 * @details Configures PLL to generate a 100 MHz system clock from 8 MHz HSE.
 */
static const RCC_SYS_CFG clk_cfg =
{
    .CLK_SOURCE =   RCC_CLK_SRC_PLL,
    .FLASH_LATENCY = 1U,
    .AHB_PRESCALER = 0U,
    .APB1_PRESCALER = 5U,
    .APB2_PRESCALER = 0U,

    .PLL =
    {
        .SRC = RCC_CLK_SRC_HSE,
        .M = 4U,
        .N = 100U,
        .P = 4U,
        .Q = 4U
    }
};

/**
 * @section Private Function Declarations.
 */
static void delay(uint32_t count);

/**
 * @section Public Function Definations.
 */

/**
 * @brief  This function is entry point of the application.
 * @return int Idealy not returns
 */
int main(void)
{
    (void)rccSystemClockConfig(&clk_cfg);

    rccEnableAHB1(RCC_AHB1ENR_GPIOAEN);

    GPIOA->MODER &= ~(3UL << (5U * 2U));
    GPIOA->MODER |=  (1UL << (5U * 2U));

    while (1)
    {
        GPIOA->ODR ^= (1UL << 5U);
        delay(1000000U);
    }
}

/**
 * @section Private Function Definations.
 */

/**
 * @brief   Simple blocking delay.
 * @param   count   Number of loop iterations.
 */
static void delay(uint32_t count)
{
    volatile uint32_t i;

    for (i = 0U; i < count; i++)
    {
        __NOP();
    }
}
