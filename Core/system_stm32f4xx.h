/**
 * @file    system_stm32f4xx.h
 * @author  Pratik Dhulubulu
 * @brief   STM32F4xx system initialization and core clock configuration.
 * @details Provides the declaration of SystemInit().
 *          SystemInit() is called at startup to configure the System Clock.
 */

#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#include <stdint.h>

void SystemInit(void);

#endif