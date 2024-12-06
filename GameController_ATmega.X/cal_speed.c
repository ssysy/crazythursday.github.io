/* 
 * File:   cal_speed.c
 * Author: 86166
 *
 * Created on 29 November 2024, 15:27
 */
#include "cal_speed.h"
#include "imu.h"  
#include "uart.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// global variables 
volatile bool integrate_flag = false; // if the programme is integrating 
volatile float velocity = 0.0;        // 
volatile uint16_t timer_tick = 0;     // timer

// Y axis speed conversion
#define G_TO_MS2 9.80665
#define SCALE_FACTOR 0.000122 // ?4g to (g/bit)

// initial timer and interrupt 
void cal_speed_init(void) {
    //every 1ms
    TCCR0A = 0;                       // normal mode
    TCCR0B = (1 << CS01) | (1 << CS00); // prescaler 64
    TIMSK0 = (1 << TOIE0);            // 
    sei();                            // 
}

// begin integration
void start_speed_calculation(void) {
    calibrate_bias(); 
    cli();                 // stop global interrupt
    integrate_flag = true; // enable integration
    velocity = 0.0;        // reset speed
    sei();                 // enable interrupt
}

// 
float stop_speed_calculation(void) {
    cli();                  // 
    integrate_flag = false; // stop integration
    sei();                  // 
    return velocity;
}

// check integration stage
bool is_calculating(void) {
    return integrate_flag;
}

// timer interrupt
ISR(TIMER0_OVF_vect) {
    if (integrate_flag) {
        // read Y axis from imu
//        uint16_t ay = IMU_ReadAxis(OUTY_L_A, OUTY_H_A);
        float acceleration = get_corrected_acceleration();

        // 
        velocity += acceleration * 0.001; // dt = 1ms = 0.001s
    }
}

//---------- calibrate_bias
static float bias = 0.0;

void calibrate_bias(void) {
    int32_t sum = 0;
    for (int i = 0; i < 100; i++) { // read 100 times
        sum += IMU_ReadAxis(OUTY_L_A, OUTY_H_A);
        _delay_ms(1); //
    }
    bias = (sum / 100.0) * SCALE_FACTOR * G_TO_MS2;
}

float get_corrected_acceleration(void) {
    
    int16_t raw_ay = IMU_ReadAxis(OUTY_L_A, OUTY_H_A); //IMU
    float acceleration = raw_ay * SCALE_FACTOR * G_TO_MS2; // to m/s?
    return acceleration - bias; //
}

//float get_corrected_pitch(void){
//    
//    float alpha = 0.98;  // filter ratio
//    float pitch_acc; //  Pitch from acc
//    float pitch;    // ???  
//
//    uint16_t ax = IMU_ReadAxis(OUTX_L_A, OUTX_H_A);
//    uint16_t ay = IMU_ReadAxis(OUTY_L_A, OUTY_H_A);
//    uint16_t az = IMU_ReadAxis(OUTZ_L_A, OUTZ_H_A);
//        
//    uint16_t gx = IMU_ReadAxis(OUTX_L_G, OUTX_H_G);
//
//// Update from acc
//pitch_acc = asin(ax / sqrt(ax * ax + ay * ay + az * az));
//
//// Update from gyro
//pitch += gx * deltaTime;
//
//// Complementray filter
//pitch = alpha * pitch + (1 - alpha) * pitch_acc;
//}
