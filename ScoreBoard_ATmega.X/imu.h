/* 
 * File:   imu.h
 * Author: Zheyu Li
 *
 * Created on November 28, 2024, 2:44 PM
 */

#ifndef IMU_H
#define IMU_H

#include <stdint.h>

// LSM6DSO32 IMU I2C address
#define IMU_ADDRESS 0x6A

// register 
#define CTRL1_XL   0x10 // accelrator regsitor
#define CTRL2_G    0x11 // gyn


// acc
#define OUTX_L_A   0x28 // X acc
#define OUTX_H_A   0x29 // X g
#define OUTY_L_A   0x2A // Y acc
#define OUTY_H_A   0x2B // Y g
#define OUTZ_L_A   0x2C // Z acc
#define OUTZ_H_A   0x2D // Z acc

// g
#define OUTX_L_G   0x22 // X
#define OUTX_H_G   0x23
#define OUTY_L_G   0x24
#define OUTY_H_G   0x25
#define OUTZ_L_G   0x26
#define OUTZ_H_G   0x27

//temp
#define OUTX_TEMP_L 0x20 // Temp
#define OUTX_TEMP_H 0x21 

// IMU initial and read
void IMU_Init(void);
uint16_t IMU_ReadAxis(uint8_t low_reg, uint8_t high_reg);

#endif


