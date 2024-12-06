/* 
 * File:   driver.c
 * Author: 86166
 *
 * Created on November 15, 2024, 2:39 PM
 * 
 * Set different ID for any Divece you want to ADD!!!
 * 
 * PitcherID 1 // HitBatID 2
 * 
 */

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

#define BUTTON_PIN PD2
#define DEVICE_ID 2 // PitcherID 1 // HitBatID 2

// define frequency
#define BAUD_RATE 9600
#define F_CPU 16000000UL
#define SCL_FREQ 400000UL

// define acc calculation
#define G_TO_MS2 9.80665    // 1g to m/s?
#define SCALE_FACTOR 0.000122 // 4g (g/bit)

#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)


void button_init(void) {
    DDRD &= ~(1 << BUTTON_PIN);  // PD2 input
    PORTD |= (1 << BUTTON_PIN);  // pull up resistor enable
    EICRA = (1 << ISC00);        // interrupt
    EIMSK = (1 << INT0);         // enable INT0
}

//button interrupt
ISR(INT0_vect) {
//    _delay_ms(2); 
    if (PIND & (1 << BUTTON_PIN)) {
        // button release
        float final_velocity = stop_speed_calculation();
        int angle = (abs(IMU_ReadAxis(OUTX_L_A, OUTX_H_A))>4000)? 1 : 0;
        
        char buffer[50];      
        char string[50];
        
        sprintf(buffer, "Bat Y Velocity: %.3f m/s, Bat_acc: %d \r\n", final_velocity, angle);
        UART_putstring(buffer);
        
        sprintf(string, "Id:%d, Vel:%.3f, Acc:%d \r\n", DEVICE_ID, final_velocity, angle); 
        UART1_putstring(string);
        
    } else {
        // button press
        start_speed_calculation();
    }
}


int main(void) {
    // initialize I2C & IMU
    I2C_Init(SCL_FREQ, F_CPU / 1000);
    IMU_Init();
    UART_init(BAUD_PRESCALER);
    UART1_init(BAUD_PRESCALER); 
    
    calibrate_bias();  
    cal_speed_init();
    button_init();

    sei(); // enable global interrupt 

    while (1) {
//        char string1[30];
//        sprintf(string1, "received\r\n"); 
//        UART1_putstring(string1);

    }
    return 0;
}

