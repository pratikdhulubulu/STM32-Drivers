/**
 * @file   main.c
 * @author Pratik Dhulubulu
 * @brief  This file initializes system clock and toggles an LED using GPIO driver APIs.
 */
#include "stm32f446xx.h"
#include "rcc_driver.h"
#include "gpio_driver.h"

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
 * @brief GPIO configuration for LED on PA5.
 */
static const GPIO_CFG led_cfg =
{
    .ptr_port  = GPIOA,
    .pin       = PIN_5,
    .mode      = GPIO_MODE_OUTPUT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_NONE,
    .alt_func  = 0U
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
    /* Configure system clock */
    (void)rccSystemClockConfig(&clk_cfg);

    /* Initialize GPIO for LED on PA5 */
    gpioInit((GPIO_CFG *)&led_cfg);

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
