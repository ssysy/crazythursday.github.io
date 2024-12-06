/* 
 * File:   driver.c
 * Author: 86166
 *
 * Created on November 15, 2024, 2:39 PM
 */
#define F_CPU 16000000UL
#define SCL_FREQ 400000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h> 
#include <string.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "old/ST7735.h"
#include "LCD_GFX.h"
#include "I2C.h"
#include "IMU.h"
#include "UART.h"
#include "UART1.h"
#include "game.h"

// define frequency
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
bool start = false;
uint8_t pwmflag = 0;


void initializeGame(Ball* ball, Catcher* catcher, Hitter* hitter) {
    lcd_init();
    LCD_setScreen(rgb565(0, 0, 0)); // Set screen to black

    // Initialize the ball
    ball->x = 80; // Centered horizontally
    ball->y = 120; // Bottom of the screen
    ball->v = 0;
    ball->a = 0;
    ball->radius = 5;
    ball->color = rgb565(255, 255, 255); // White ball
    ball->moving = false;
    ball->score = 0;
    ball->dispal = false;

    // Initialize the catcher
    catcher->x = 70;
    catcher->y = 10; // Near the top of the screen
    catcher->width = 20;
    catcher->height = 10;
    catcher->color = rgb565(0, 255, 0); // Green catcher

    // Initialize the hitter piston
    hitter->x = ball->x - 10; // Centered below the ball
    hitter->y = ball->y + 10; // Positioned slightly below the ball
    hitter->width = 20;
    hitter->height = 5;
    hitter->color = rgb565(0, 255, 0); // Green piston
    hitter->score = 0;
}

void parse_input(uint8_t input){
    uint8_t bit0 = (input >> 0) & 0x01;
    uint8_t bit1 = (input >> 1) & 0x01;
    uint8_t bit2 = (input >> 2) & 0x01;
    uint8_t bit3 = (input >> 3) & 0x01;
    uint8_t bit4 = (input >> 4) & 0x01;
    uint8_t bit5 = (input >> 5) & 0x01;
    uint8_t bit6 = (input >> 6) & 0x01;
    uint8_t bit7 = (input >> 7) & 0x01;
    


    printf("Bit 7 (MSB): %u\n", bit7);
    printf("Bit 6: %u\n", bit6);
    printf("Bit 5: %u\n", bit5);
    printf("Bit 4: %u\n", bit4);
    printf("Bit 3: %u\n", bit3);
    printf("Bit 2: %u\n", bit2);
    printf("Bit 1: %u\n", bit1);
    printf("Bit 0 (LSB): %u\n", bit0);
    if(bit7){
        start = true;
    }
}


void pwmInit (void){
    
    DDRC |= (1 << PORTC4); //LED output
    DDRC |= (1 << PORTC3); //LED
    
    DDRC |= (1 << PORTC5); //Buzzer PB2
//    DDRC &=~ (1 << PORTC5); 
    
// Timer3 PWM
//    DDRD |= (1 << DDD0); //PD0
//    
//    TCCR3A |= (1 << WGM30) | (1 << WGM32);  // 
//    TCCR3B |= (1 << WGM32) | (1 << CS32);   // 64
//    TCCR3A |= (1 << COM3A1);    // OC3A-PD0 //turn ON PWM
//    OCR3A = 180;

}

int main(void) {
    Ball bal;
    Catcher cat;
    Hitter hit;
    GameState st;
    
    Ball* ball = &bal;
    Catcher* catcher = &cat;
    Hitter* hitter = &hit;
    GameState* state = &st;
    *state = IDLE;

    
    initializeGame(ball, catcher, hitter);
    pwmInit();
    
    // init I2C & IMU
//    I2C_Init(SCL_FREQ, F_CPU / 1000);
//    IMU_Init();
    UART_init(BAUD_PRESCALER); 
    UART1_init(BAUD_PRESCALER);
    
//    char string[100];
//    char string1[50];
    unsigned char received_data;
//    LCD_drawChar(40, 40, '0', WHITE, BLACK);
//    drawscore(ball, hitter);
//    if (*state == IDLE) {
//        LCD_displayStringSPI(42, 110, "Press to Start!", rgb565(255, 255, 255), rgb565(0, 0, 0));
//        _delay_ms(1000);
//        received_data = UART1_Receive();
//        char string[30] = "";
//        sprintf(string, "received: %d", received_data);
//        UART_putstring(string);
//        if (((received_data >>7) & 0x01) == 1){
//            *state = PREPARE;
//            UART_putstring("To Prepare State");
//        }
////            LCD_drawBlock(42, 110, 140, 140, rgb565(0, 0, 0));
//    }
    
    while (1) {
        
        if (*state == IDLE) {
            if(!(ball->dispal)){
                drawscore(ball, hitter);
                ball->dispal = true;
            }
            LCD_displayStringSPI(42, 110, "Press to Start!", rgb565(255, 255, 255), rgb565(0, 0, 0));
            _delay_ms(500);
            received_data = UART1_Receive();
            char string[30] = "";
            sprintf(string, "received: %d", received_data);
            UART_putstring(string);
            if (((received_data >>7) & 0x01) == 1){
                *state = PITCHING;
                UART_putstring("To Prepare State");
                LCD_setScreen(rgb565(0, 0, 0));
            }else if (((received_data >>6) & 0x01) == 1 && (((received_data >>5) & 0x01) != 1) ){

            PORTC |= (1 << PORTC5); //ON BUZZER
                // Buzzer play sound
            }
            

//            LCD_drawBlock(42, 110, 140, 140, rgb565(0, 0, 0));
        }
        
//        if (pwmflag == 0) 
        game(ball, catcher, hitter, state, &received_data);
        pwmflag = 1;
        
        _delay_ms(800);
        
//        TCCR2A &=~ (1 << COM2B1);//Turn OFF pwm.
        
        PORTC &=~ (1 << PORTC5); //OFF BUZZER
        PORTC &=~ (1 << PORTC3); //OFF led
        PORTC &=~(1 << PORTC4); //OFF led
        
//        receive data from esp32
//        *received_data = UART1_Receive();
////        parse_input(received_data);
//        if(start){
//            *state = PREPARE;
//        }
//        char string[30] = "";
        
//        sprintf(string, "received: %d", *received_data);
//        UART_putstring(string);
//        
        
//        if (((*received_data >>7) & 0x01) == 1){
//            *state = PREPARE;
//            UART_putstring("To Prepare State");
//            if ((received_data & 0x01) == 1){
//                hitter->score ++;
//                if((received_data >>6) & 0x01 == 1){
//                    hitter->score ++;
//                }
//            }
//            else{
//                ball->score ++; 
//                if((received_data >>6) & 0x01 == 1){
//                    ball->score ++;
//                }
//            }
//        }
        
//        if (hitter->score > 5 || ball->score >5){
//            hitter->score = 0;
//            ball->score = 0;
//        }
        
//        reset();
//        drawscore(ball, hitter);
    
        
////         send data to PC
//        sprintf(string1, "Received from ESP32: %d \r\n", received_data);
////        sprintf(string1,"%d\r\n", received_data);
//        UART_putstring(string1);
        
    }
        
//        parse_input(received_data);

    return 0;
}



