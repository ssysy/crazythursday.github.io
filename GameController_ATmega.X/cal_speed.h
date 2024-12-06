/* 
 * File:   cal_speed.h
 * Author: 86166
 *
 * Created on 29 November 2024, 15:42
 */

#ifndef CAL_SPEED_H
#define CAL_SPEED_H

#include <stdint.h>
#include <stdbool.h>

// initial timer and speed
void cal_speed_init(void);

// begin speed cal
void start_speed_calculation(void);

// integration end
float stop_speed_calculation(void);

// stop sign
bool is_calculating(void);

//calibrate
void calibrate_bias(void);
float get_corrected_acceleration(void);

#endif // CAL_SPEED_H


