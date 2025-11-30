/**
 * @file   main.c
 * @author Pratik Dhulubulu
 * @brief  This file initializes PWM and controls on-board LED.
 */
#include "stm32f446xx.h"
#include "rcc_driver.h"
#include "gpio_driver.h"
#include "timer_driver.h"

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
    .mode      = GPIO_MODE_ALT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_NONE,
    .alt_func  = 1U
};

/**
 * @brief TIM2 PWM configuration for LED.
 */
static const TIM_CONFIG tim2_pwm_cfg =
{
    .ptr_tim   = TIM2,
    .clock_hz  = 50000000u,
    .mode      = TIM_MODE_PWM,
    .prescaler = 49u,
    .period    = 999u,
    .channel   = TIM_CHANNEL_1,
    .pulse     = 700u,
    .polarity  = 0u
};

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
    
    /* Configure TIM2 clock */
    timerInit(&tim2_pwm_cfg);
    tim2_pwm_cfg.ptr_tim->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    timerStart(&tim2_pwm_cfg);

    while (1)
    {
    }
}
