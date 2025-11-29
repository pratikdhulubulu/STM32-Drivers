/**
 * @file    stm32f4xx_it.h
 * @author  Pratik Dhulubulu
 * @brief   Interrupt Service Routine Interface for STM32F446xx.
 * @details Declares core and peripheral interrupt handlers required
 *          by the vector table.
 */

#ifndef STM32F4XX_IT_H
#define STM32F4XX_IT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @section Cortex-M4 Processor Exceptions.
 */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/**
 * @section Peripheral Interrupt Handlers.
 */
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
