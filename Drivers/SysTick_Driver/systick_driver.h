/**
 * @file    systick_driver.h
 * @author  Pratik Dhulubulu
 * @brief   SysTick driver interface for STM32F446xx.
 */
#ifndef SYSTICK_DRIVER_H
#define SYSTICK_DRIVER_H

#include <stddef.h>
#include <stdint.h>
#include "stm32f446xx.h"

/** 
 * @section Public Data Declarations.
 */
extern volatile uint32_t tick;

/** 
 * @section Public Function Declarations.
 */
void sysTickInit(uint32_t ticks);
void sysTickDelayMs(uint32_t ms);
uint32_t sysTickGetTick(void);
void sysTickHandleIrq(void);

#endif
