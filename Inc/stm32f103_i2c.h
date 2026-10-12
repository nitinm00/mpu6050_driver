/*
 * stm32f103_i2c.h
 *
 *  Created on: Sep 22, 2026
 *      Author: nitin
 */

#ifndef STM32F103_I2C_H_
#define STM32F103_I2C_H_

#include "stm32f103_gpio.h"
/*
 * PB6 -> I2C1_SCL
 * PB7 -> I2C1_SDA
 */

#define APB1_OFFSET 		(0x00U)
#define APB1_PERIPH_BASE 	(PERIPH_BASE + APB1_OFFSET)
#define APB1_EN_R_OFFSET 	(0x1CUL)
#define RCC_APB1_EN_R 		(*(volatile unsigned long*) (APB1_PERIPH_BASE + APB1_EN_R_OFFSET))

#define I2C1_OFFSET 		(0x5400UL)
#define I2C1_BASE 			(APB1_PERIPH_BASE + I2C1_OFFSET)

#define GPIOB_OFFSET 		(0x0C00UL)
#define GPIOB_BASE 			(APB2_PERIPH_BASE + GPIOB_OFFSET)

#define GPIOX_CRL 			(0x00U)
#define GPIOB_MODE_R_OFFSET GPIOX_CRL
#define GPIOB_MODE_R 		(*(volatile unsigned long*) (GPIOB_BASE + GPIOB_MODE_R_OFFSET))

#define I2C_CR1_OFFSET		(0x00U)
#define I2C_CR1				(*(volatile unsigned long*) (I2C1_BASE + I2C1_CR1_OFFSET))

#define I2C_CR2_OFFSET		(0x04U)
#define I2C_CR2				(*(volatile unsigned long*) (I2C1_BASE + I2C_CR2_OFFSET))

#define I2C_CCR_OFFSET 		(0x1CU)
#define I2C_CCR				(*(volatile unsigned long*) (I2C1_BASE + I2C_CCR_OFFSET))

#define I2C_100KHZ			80U
#define SD_MODE_MAX_RISE_TIME	17U

#define I2C_TRISE_OFFSET	(0x20U)
#define I2C1_TRISE			(*(volatile unsigned long*) (I2C1_BASE + I2C_TRISE_OFFSET))

#define GPIOB_EN 			(1U << 3)
#define I2C1_EN 			(1U << 21)

#define CR1_PE				(1U << 0)
#define CR1_START			(1U << 8)
#define CR1_STOP			(1U << 9)

#define I2C1_DR				(*(volatile unsigned long*) (I2C1_BASE + 0x10U))
#define I2C1_SR2			(*(volatile unsigned long*) (I2C1_BASE + 0x18U))
#define SR2_BUSY			(1U << 1)

#define SR1_START			(1U << 0)
#define SR1_ADDR			(1U << 1)

#define SR1_TXE				(1U << 7)
#define SR1_RXNE			(1U << 6)
#define SR1_BTF				(1U << 2)
#define CR1_ACK 			(1U << 10)

void i2c1_init(void);
void i2c1_byte_read(char slave_addr, char mem_addr, char* data);
void i2c1_burst_read(char slave_addr, char mem_addr, unsigned int n, char* data);
void i2c1_burst_write(char slave_addr, char mem_addr, unsigned int n, char* data);





#endif /* STM32F103_I2C_H_ */
