/*
 * stm32f103_i2c.c
 *
 *  Created on: Sep 22, 2026
 *      Author: nitin
 */

#include "stm32f103_i2c.h"

/*
 * PB6 -> I2C1_SCL
 * PB7 -> I2C1_SDA
 */

void i2c1_init(void)
{
	// enable clock for GPIOB
	RCC_APB2EN_R |= GPIOB_EN;

	// set GPIO B6 and B7 mode to alternate function open-drain
	GPIOB_MODE_R |= (1U << 27);
	GPIOB_MODE_R |= (1U << 26);

	GPIOB_MODE_R |= (1U << 31);
	GPIOB_MODE_R |= (1U << 30);

	// set GPIO B6 and B7 to 10MHz
	GPIOB_MODE_R &= ~(1U << 25);
	GPIOB_MODE_R |= (1U << 24);

	GPIOB_MODE_R &= ~(1U << 29);
	GPIOB_MODE_R |= (1U << 28);

	// enable clock for I2C1
	RCC_APB1_EN_R |= I2C1_EN;

	// use external 1k pull up resistor as F1 series doesn't have GPIO output pullup-pulldown setting

	// set I2C to reset mode
	I2C_CR1 |= (1U << 15);

	// exit reset mode
	I2C_CR1 &= ~(1U << 15);

	// set peripheral clock frequency
	I2C_CR2 |= (1U << 4); // 16 MHz

	// set I2c to standard mode (100kHz)
	I2C_CCR = I2C_100KHZ;

	// set rise time
	I2C1_TRISE = SD_MODE_MAX_RISE_TIME;

	// enable
	I2C_CR1 = CR1_PE;

	return 0;
}

void i2c1_byte_read(char slave_addr, char mem_addr, char* data)
{

	volatile int tmp;

	// wait until bus is not busy
	while (I2C1_SR2 & (SR2_BUSY)) {}

	// set start bit
	I2C_CR1 |= CR1_START;

	// wait until status shows start bit is set
	// while it is not started, wait
	while (!(I2C_SR1 & SR1_START)) {}

	// request access to slave addr by sending its address on bus along with write bit
	// shift left by 1 to make room for R/W bit in LSB position (0 -> write, 1 -> read)
	I2C1_DR |= ( (slave_addr << 1) | 0U);

	// wait until status shows slave address sent, ACK received
	// while address status bit not set, wait
	while (!(I2C_SR1 & SR1_ADDR)) {}

	// clear ADDR flag from SR1
	// documented in SR2 note
	tmp = I2C1_SR2;

	// transmit memory address
	I2C1_DR |= mem_addr;

	// wait until transmitter is empty
	// while TxE is not empty, wait
	while (!(I2C_SR1 & SR1_TXE)) {}

	// generate repeat start condition
	I2C_CR1 |= CR1_START;

	// wait until status shows start bit is set
	while (!(I2C_SR1 & SR1_START)) {}

	// read from slave addr
	I2C1_DR |= (slave_addr << 1 | 1U);

	// wait until status shows slave address sent, ACK received
	// while address status bit not set, wait
	while (!(I2C_SR1 & SR1_ADDR)) {}

	// disable acknowledgment to prepare for STOP condition
	I2C_CR1 &= ~CR1_ACK;

	// clear ADDR flag from SR1
	tmp = I2C1_SR2;

	// generate stop condition
	I2C_CR1 |= CR1_STOP;

	// wait until receiver is NOT empty (indicating there is data ready to be read)
	while (!(I2C_SR2 & SR2_RXNE)) {}

	// read data from DR
	*data++ = I2C1_DR;

}

/**
 * Function to burst read from I2C slave device
 * @param slave_addr: address of I2C slave device
 * @param mem_addr: memory address to read data from
 * @param n: Number of bytes to read
 * @param data: buffer to store data into
 */
void i2c1_burst_read(char slave_addr, char mem_addr, unsigned int n, char* data)
{

	// to clear SR2
	volatile int tmp;

	// wait until bus is not busy
	while (I2C1_SR2 & (SR2_BUSY)) {}

	// set start bit
	I2C_CR1 |= CR1_START;

	// wait until status shows start bit is set
	while (!(I2C_SR1 & SR1_START)) {}

	// transmit slave addr + write
	I2C1_DR |= (slave_addr << 1 | 0U);

	// wait until status shows slave address sent, ACK received
	while (!(I2C_SR1 & SR1_ADDR)) {}

	// clear ADDR flag from SR1
	tmp = I2C1_SR2;

	// transmit memory address
	I2C1_DR |= mem_addr;

	// wait until transmitter is empty
	while (!(I2C_SR1 & SR1_TXE)) {}

	// generate repeat start condition
	I2C_CR1 |= CR1_START;

	// wait until start bit is set
	while (!(I2C_SR1 & SR1_START)) {}

	// transmit slave addr + read
	I2C1_DR |= (slave_addr << 1 | 1U);

	// wait until status shows slave address sent, ACK received
	while (!(I2C_SR1 & SR1_ADDR)) {}

	// clear addr flag
	tmp = I2C_SR2;

	// enable acknowledge to keep bus open (?)
	I2C_CR1 |= CR1_ACK;

	while (n > 0U) {
		if (n == 1)
		{
			// disable acknowledgment to prepare for STOP condition
			I2C_CR1 &= ~CR1_ACK;

			// generate stop condition
			I2C_CR1 |= CR1_STOP;

			// wait until data is available
			while (!(I2C_SR2 & SR2_RXNE)) {}

			// read data from DR
			*data++ = I2C1_DR;

			break;
		}
		else
		{
			// wait until data is available
			while (!(I2C_SR2 & SR2_RXNE)) {}

			(*data++) = I2C1_DR;

			n--;
		}
	}
}

void i2c_burst_write(char slave_addr, char mem_addr, unsigned int n, char* data)
{
	// to clear SR2
	volatile int tmp;

	// wait until bus is not busy
	while (I2C1_SR2 & (SR2_BUSY)) {}

	// set start bit
	I2C_CR1 |= CR1_START;

	// wait until status shows start bit is set
	while (!(I2C_SR1 & SR1_START)) {}

	// transmit slave addr + write
	I2C1_DR |= (slave_addr << 1 | 0U);

	// wait until status shows slave address sent, ACK received
	while (!(I2C_SR1 & SR1_ADDR)) {}

	// clear ADDR flag from SR1
	tmp = I2C1_SR2;

	// wait until transmitter is empty
	while (!(I2C_SR1 & SR1_TXE)) {}

	// transmit memory address
	I2C1_DR |= mem_addr;

	for (unsigned int i = 0; i < n; i++)
	{
		// wait until transmitter is empty
		while (!(I2C_SR1 & SR1_TXE)) {}
		// transmit data
		I2C1_DR |= *data++;
	}

	// wait for byte transfer to finish
	while (!(I2C_SR1 & SR1_BTF)) {}

	// generate STOP condition
	I2C_CR1 |= CR1_STOP;
}











