/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   Example usage of SysTick driver on STM32F446.
 * @details Demonstrates tick counting, delay, and global tick access.
 */
#include "stm32f446xx.h"
#include "exti_driver.h"
#include "gpio_driver.h"
#include "rcc_driver.h"
#include "systick_driver.h"

/**
 * @section Private Data Definations.
 */

/**
 * @brief   System clock configuration.
 * @details Configures PLL to generate a 50 MHz system clock from 8 MHz HSE.
 */
static const RCC_SYS_CFG clk_cfg =
{
    .CLK_SOURCE =   RCC_CLK_SRC_PLL,
    .FLASH_LATENCY = 1U,
    .AHB_PRESCALER = 0U,
    .APB1_PRESCALER = 4U,
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
 * @brief GPIO configuration for Button on PC13.
 */
static const GPIO_CFG button_cfg =
{
    .ptr_port  = GPIOC,
    .pin       = PIN_13,
    .mode      = GPIO_MODE_INPUT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_UP,
    .alt_func  = 0U
};

/**
 * @brief EXTI Cinfiguration for Button on PA13
 */
static const EXTI_CONFIG exti_cfg =
{
    .ptr_port   = GPIOC,
    .line       = EXTI_LINE_13,
    .trigger    = EXTI_TRIGGER_FALLING,
    .priority   = 2
};

/**
 * @section Private Function Declarations.
 */
static void buttonPushIsr(void);

/**
 * @section Public Function Definations.
 */

 /**
  * @brief  This function will initialize peripherals and runs appication.
  * @param  None.
  * @return int Idealy not returns.
  */
int main(void)
{
    /* Configure system clock */
    (void)rccSystemClockConfig(&clk_cfg);

    /* Configure SysTick for 1 ms */
    sysTickInit(SystemCoreClock/1000);

    /* Initialize GPIO for LED on PA5 */
    gpioInit((GPIO_CFG *)&led_cfg);

    /* Initialize GPIO for Button on PC13 */
    gpioInit((GPIO_CFG *)&button_cfg);

    /* Initialize EXTI for Button on PA13 */
    extiInit(&exti_cfg);
    extiRegisterCallback(EXTI_LINE_13, buttonPushIsr);

    while (1)
    {

    }
}

/**
 * @section Private Function Definations.
 */

/**
 * @brief  This function is handler for button push interrupt, toggles the LED state.
 * @param  None.
 * @retuen None.
 */
static void buttonPushIsr(void)
{
    gpioWritePin(GPIOA, PIN_5, 1);
    sysTickDelayMs(4000);
    gpioWritePin(GPIOA, PIN_5, 0);
}
