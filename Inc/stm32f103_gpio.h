/*
 * stm32f104_gpio.h
 *
 *  Created on: Sep 22, 2026
 *      Author: nitin
 */

#ifndef STM32F103_GPIO_H_
#define STM32F103_GPIO_H_

/* GPIO setup (LED) */

#define PERIPH_BASE 			(0x40000000UL)
#define APB2_OFFSET 			(0x10000UL)
#define APB2_PERIPH_BASE		(PERIPH_BASE + APB2_OFFSET)

#define GPIOC_OFFSET 			(0x1000UL)
#define GPIOC_BASE 				(APB2_PERIPH_BASE + GPIOC_OFFSET)

#define RCC_OFFSET 				(0x21000UL)
#define RCC_BASE 				(PERIPH_BASE + RCC_OFFSET)

#define APB2_ENR_OFFSET 		(0x18UL)
#define RCC_APB2EN_R  			(*(volatile unsigned long *)  (RCC_BASE + APB2_ENR_OFFSET))

#define GPIOX_CRH_OFFSET		(0x04UL)
#define MODE_R_OFFSET 			GPIOX_CRH_OFFSET
#define GPIOC_MODE_R 			(*(volatile unsigned long *) (GPIOC_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET				(0x0CUL)
#define GPIOC_OD_R				(*(volatile unsigned long *) (GPIOC_BASE + OD_R_OFFSET))

// bit mask for enabling GPIOC (bit 4)
#define GPIOCEN 				(1U << 4)
#define PIN_13					(1U << 13)
#define LED						PIN_13



#endif /* STM32F103_GPIO_H_ */
