#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <string.h>

#include "I2C.h"
#include "imu.h"
#include "uart.h"
#include "uart1.h"
#include "cal_speed.h"

float alpha = 0.98;  // ????
float pitch_acc, roll_acc; // ???????? Pitch ? Roll
float pitch, roll, yaw;    // ???  

    uint16_t ax = IMU_ReadAxis(OUTX_L_A, OUTX_H_A);
    uint16_t ay = IMU_ReadAxis(OUTY_L_A, OUTY_H_A);
    uint16_t az = IMU_ReadAxis(OUTZ_L_A, OUTZ_H_A);
        
    uint16_t gx = IMU_ReadAxis(OUTX_L_G, OUTX_H_G);

// ???????
pitch_acc = asin(ax / sqrt(ax * ax + ay * ay + ay * ay));

// ?????????????
pitch += gx * deltaTime;

// ???????
pitch = alpha * pitch + (1 - alpha) * pitch_acc;
