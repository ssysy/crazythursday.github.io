#include <avr/io.h>
#include "I2C.h"

// Initialize I2C (TWI)
void I2C_Init(uint8_t scl_frequency, uint8_t cpu_frequency) {
    uint8_t prescaler = 0; // Use prescaler value of 1
    TWSR0 = prescaler;      // Set the prescaler bits in TWSR
    TWBR0 = ((cpu_frequency / scl_frequency) - 16) / (2 * (1 << (2 * prescaler)));

    // Enable TWI
    TWCR0 = (1 << TWEN);
}

// Send START condition
void I2C_Start(void) {
    TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); // Generate START condition
    while (!(TWCR0 & (1 << TWINT)));                            // Wait for START condition to be transmitted
    uint8_t status = TWSR0 & 0xF8;
    if (status != 0x08) {
        // Error handling: START condition not successfully transmitted
    }
}

// Send slave address
void I2C_SendAddress(uint8_t address, uint8_t read_write) {
    TWDR0 = (address << 1) | (read_write & 0x01); // Write the slave address and read/write bit
    TWCR0 = (1 << TWEN) | (1 << TWINT);          // Start transmission
    while (!(TWCR0 & (1 << TWINT)));             // Wait for operation to complete

    uint8_t status = TWSR0 & 0xF8;
    if (status != 0x40) { // Check if master receive mode was successfully entered
        // Error handling: Slave did not acknowledge or address not confirmed
    }
}

// Read data
uint8_t I2C_ReadData(uint8_t ack) {
    if (ack) {
        TWCR0 = (1 << TWEN) | (1 << TWEA) | (1 << TWINT); // Send ACK
    } else {
        TWCR0 = (1 << TWEN) | (1 << TWINT);              // Send NACK
    }
    while (!(TWCR0 & (1 << TWINT)));                     // Wait for data reception to complete
    return TWDR0;                                        // Return received data
}

void I2C_WriteData(uint8_t data) {
    // Write data to TWDR
    TWDR0 = data;

    // Clear TWINT to start transmission
    TWCR0 = (1 << TWEN) | (1 << TWINT);

    // Wait for operation to complete
    while (!(TWCR0 & (1 << TWINT)));

    // Check status
    if ((TWSR0 & 0xF8) != 0x28) {
        // Error handling: Data transmission failed
    }
}

// Send STOP condition
void I2C_Stop(void) {
    TWCR0 = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); // Generate STOP condition
}

