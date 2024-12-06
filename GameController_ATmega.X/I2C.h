#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

/**
 * I2C Port (TWI0/TWCR0)
 */

#define I2C_SDA PC4
#define I2C_SCL PC5

void I2C_Init(uint8_t scl_frequency, uint8_t cpu_frequency);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAddress(uint8_t address, uint8_t read_write);
void I2C_WriteData(uint8_t data);
uint8_t I2C_ReadData(uint8_t ack);

#endif
