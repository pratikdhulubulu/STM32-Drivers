/**
 * @file    systick_driver.c
 * @author  Pratik Dhulubulu
 * @brief   SysTick driver implementation for STM32F446xx.
 * @details This file provides SysTick initialization, millisecond delay,
 *          and tick retrieval functions. 
 */
#include "systick_driver.h"
#include "stm32f446xx.h"

/** 
 * @section Public Data Definations.
 */
volatile uint32_t tick = 0u;

/** 
 * @section Public Function Definations.
 */

/** 
 * @brief This function initialize SysTick.
 * @param tick Number of SysTick counts for 1 timer period.
 * @return None.
 */
void sysTickInit(uint32_t ticks)
{
    SysTick->LOAD = ticks - 1u;
    SysTick->VAL  = 0u;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk |
                    SysTick_CTRL_CLKSOURCE_Msk;
}

/**
 * @brief  This Function Provides blocking delay for specified milliseconds.
 * @param  ms Number of milliseconds to delay.
 * @return None
 */
void sysTickDelayMs(uint32_t ms)
{
    uint32_t m = tick;
    while (tick-m < ms)
    {
        __NOP();
    }
}

/**
 * @brief This function returns the current SysTick tick count.
 * @return tick Current tick count
 */
uint32_t sysTickGetTick(void)
{
    return tick;
}
