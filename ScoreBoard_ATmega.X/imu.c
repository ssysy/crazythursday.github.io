/* 
 * File:   imu.c
 * Author: 86166
 *
 * Created on November 15, 2024, 3:35 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "IMU.h"
#include "I2C.h" //

//  IMU
void IMU_Init(void) {
    //1.66 kHz , ?4g
    I2C_Start();
    I2C_SendAddress(IMU_ADDRESS, 0); // write mode
    I2C_WriteData(CTRL1_XL);// acc
    I2C_WriteData(0x60); // ODR = 1.66kHz, FS = ?2g
    I2C_Stop();

    // 1.66 kHz , 250 dps
    I2C_Start();
    I2C_SendAddress(IMU_ADDRESS, 0); // write mode
    I2C_WriteData(CTRL2_G); //g
    I2C_WriteData(0x60); // ODR = 1.66kHz, FS = 250 dps
    I2C_Stop();
    
}

// read from one axis
uint16_t IMU_ReadAxis(uint8_t low_reg, uint8_t high_reg) {
    uint8_t low_byte, high_byte;

    // read low
    I2C_Start();
    I2C_SendAddress(IMU_ADDRESS, 0); // write mode
    I2C_WriteData(low_reg);          // select register 
    I2C_Start();                     // re start
    I2C_SendAddress(IMU_ADDRESS, 1); // read mode
    low_byte = I2C_ReadData(1);      // read, ACK

    // read high
    I2C_Start();
    I2C_SendAddress(IMU_ADDRESS, 0); // write mode
    I2C_WriteData(high_reg);         // select register 
    I2C_Start();                     // restart
    I2C_SendAddress(IMU_ADDRESS, 1); // write mode
    high_byte = I2C_ReadData(0);     // read, NACK
    I2C_Stop();

    return ((uint16_t)high_byte << 8) | low_byte;
}
