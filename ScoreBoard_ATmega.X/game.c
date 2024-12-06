#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h> 
#include "LCD_GFX.h"
#include "game.h"

bool singlemessage = true;
// Draw the ball
void drawBall(Ball* ball) {
    LCD_drawCircle(ball->x, ball->y, ball->radius, ball->color);
}

// Clear the ball
void clearBall(Ball* ball) {
    LCD_drawCircle(ball->x, ball->y, ball->radius, rgb565(0, 0, 0)); // Black background
}

// Draw the catcher
void drawCatcher(Catcher* catcher) {
    LCD_drawBlock(catcher->x, catcher->y, catcher->x + catcher->width, catcher->y + catcher->height, catcher->color);
}

void drawCatcherShape(uint8_t x, uint8_t y, uint16_t bodyColor, uint16_t maskColor, uint16_t gloveColor) {
    // Catcher's mask (circle for the head)
    LCD_drawCircle(x + 10, y + 5, 8, maskColor); // Head at the top

    // Catcher's body (rectangle)
    LCD_drawBlock(x, y + 15, x + 20, y + 40, bodyColor); // Body below the head

    // Catcher's arms (lines)
    LCD_drawLine(x - 10, y + 20, x, y + 25, bodyColor); // Left arm
    LCD_drawLine(x + 20, y + 25, x + 30, y + 20, bodyColor); // Right arm

    // Catcher's glove (circle on the right arm)
    LCD_drawCircle(x + 35, y + 20, 5, gloveColor); // Glove at the end of the right arm

    // Catcher's legs (rectangles)
    LCD_drawBlock(x, y + 40, x + 7, y + 60, bodyColor); // Left leg
    LCD_drawBlock(x + 13, y + 40, x + 20, y + 60, bodyColor); // Right leg
}

// Clear the catcher
void clearCatcher(Catcher* catcher) {
    LCD_drawBlock(catcher->x, catcher->y, catcher->x + catcher->width, catcher->y + catcher->height, rgb565(0, 0, 0));
}

// Draw the hitter piston
void drawHitter(Hitter* hitter) {
    LCD_drawBlock(hitter->x, hitter->y, hitter->x + hitter->width, hitter->y + hitter->height, hitter->color);
}

// Clear the hitter piston
void clearHitter(Hitter* hitter) {
    LCD_drawBlock(hitter->x, hitter->y, hitter->x + hitter->width, hitter->y + hitter->height, rgb565(0, 0, 0));
}

// Display a message
void displayMessage(char* message, uint16_t color) {
//    LCD_setScreen(rgb565(0, 0, 0));
    LCD_setScreen(rgb565(0, 0, 0));
    LCD_drawString(20, 63, message, color, rgb565(0, 0, 0));
    _delay_ms(5000);
//    LCD_drawBlock(10, 80, "", color, rgb565(0, 0, 0));
}

void drawscore(Ball* ball, Hitter* hitter){
    LCD_setScreen(rgb565(0, 0, 0));
    LCD_displayStringSPI(33, 22, "Pitcher", rgb565(255, 255, 255), rgb565(0, 0, 0));
    LCD_displayStringSPI(113, 22, "Hitter", rgb565(255, 255, 255), rgb565(0, 0, 0));
    LCD_drawChar_size(33, 52, ball->score + '0', WHITE, BLACK, 3);  // Left half score
    LCD_drawChar_size(113, 52, hitter->score + '0', WHITE, BLACK, 3); // Right half score
}

void reset(Ball* ball, Hitter* hitter){
    LCD_setScreen(rgb565(0, 0, 0));
    
    ball->score = 0;
    hitter->score = 0;
}

// Simulate the ball movement
bool moveBallUp(Ball* ball) {
    clearBall(ball);

    if (ball->y > 15) {             // Ensure the ball stops above 0
        ball->y -= 2;              // Move ball upward
        drawBall(ball);

        return true;               // Ball is still in motion
    } else {
        return false;              // Ball has reached the top
    }
}

// Main game logic
void game(Ball* ball, Catcher* catcher, Hitter* hitter, GameState* state, unsigned char* received_data) {
//    state = PITCHING;
    switch (*state) {
//        case PREPARE:
//
////            displayMessage("Preparing!", rgb565(255, 255, 255));
////            ball->x = 64;                // Center ball horizontally
////            ball->y = 120;               // Reset ball position at the bottom
////            hitter->x =  64;     // Center hitter around the ball
////            hitter->y =  10;    // Place hitter above the ball
////            ball->moving = false;
//
//            // Draw ball and hitter
//            // Simulate input to start pitching
//            if (((*received_data >>7) & 0x01) == 1) {
////                LCD_setScreen(rgb565(0, 0, 0));
//                drawBall(ball);
////                drawHitter(hitter);
//                ball->moving = true;
//                *state = PITCHING;
//                clearBall(ball);
////                displayMessage("", rgb565(0, 0, 0)); // Clear message
//            }
//            break;

        case PITCHING:
            
            ball->moving = true;
            
            if(singlemessage){
                LCD_setScreen(rgb565(0, 0, 0));
                displayMessage("Pitching!", rgb565(255, 255, 255));
                singlemessage = false;
            }
            if (ball->moving) {
                LCD_setScreen(rgb565(0, 0, 0));
                bool stillMoving = moveBallUp(ball);
                while(stillMoving){
                    stillMoving = moveBallUp(ball);
                    _delay_ms(50);
                }
                if (!stillMoving) {
                    *state = HIT;  // Switch to HIT state for external input
                    ball->moving = false;
                    clearBall(ball);
                }
            }
            break;

        case HIT:
            LCD_setScreen(rgb565(0, 0, 0));
//            displayMessage("Player Hit?", rgb565(255, 255, 0));
            //drawBall(ball);        // Ensure the ball remains visible
            //_delay_ms(1000);       // Simulate a delay to read input

            // Replace the following with your actual hit detection logic
//            if ((received_data & 0x01) == 1){
//                hitter->score ++;
//                if((received_data >>6) & 0x01 == 1){
//                    hitter->score ++;
//                }
//            }
            LCD_setScreen(rgb565(0, 0, 0));
            if ((*received_data & 0x01) == 1) {
                if((*received_data >>1) & 0x01 == 1){
                    displayMessage("Super Hit!", rgb565(255, 0, 0)); // Green text for hit
                    _delay_ms(1000);
                    hitter->score = hitter->score + 2;          // Increment score for a successful hit
                }else{
                    displayMessage("Hit!", rgb565(255, 165, 0)); // Green text for hit
                    _delay_ms(1000);
                    hitter->score = hitter->score + 1; 
                }
                //TCCR2A |= (1 << COM2B1);//Turn on pwm
                PORTC |= (1 << PORTC4); //ON led
                
            } else {
                displayMessage("Miss!", rgb565(255, 255, 0)); // Yellow text for no hit
                _delay_ms(1000);
                if((*received_data >>1) & 0x01 == 1){
                    displayMessage("Super Throw!", rgb565(255, 0, 0));
                    ball->score = ball->score + 2; 
                }else{
                    ball->score = ball->score + 1; 
                }
                
                PORTC |= (1 << PORTC3); //ON led 
                         // Increment score for a successful hit
            }
            *state = IDLE;
            ball->dispal = false;
            ball->x = 80; // Centered horizontally
            ball->y = 120; // Bottom of the screen
            singlemessage = true;
//            drawscore(ball, hitter);
            break;

        case CATCHER:
//            displayMessage("Catcher!", rgb565(255, 255, 255));
            clearBall(ball);       // Clear previous ball position
            clearHitter(hitter);
            uint8_t catcherX = 50;         // X-coordinate of the top-left corner
            uint8_t catcherY = 50;         // Y-coordinate of the top-left corner
            uint16_t bodyColor = rgb565(255, 0, 0);  // Red for body
            uint16_t maskColor = rgb565(0, 0, 255);  // Blue for mask
            uint16_t gloveColor = rgb565(255, 165, 0); // Orange for glove

            // Draw the catcher
            drawCatcherShape(catcher->x, catcher->y, bodyColor, maskColor, gloveColor);
//            drawCatcher(catcher);  // Draw the catcher

            // Collision detection between ball and catcher
            if (ball->x >= catcher->x && ball->x <= catcher->x + catcher->width &&
                ball->y >= catcher->y && ball->y <= catcher->y + catcher->height) {
                LCD_setScreen(rgb565(0, 0, 0));
                displayMessage("Caught!", rgb565(0, 255, 0)); // Green text for caught
                catcher->score = hitter->score + 1;          // Increment score for a successful catch
            } else {
                LCD_setScreen(rgb565(0, 0, 0));
                displayMessage("Missed!", rgb565(255, 0, 0)); // Red text for missed
            }

            // Display score
            char scoreText[16];
//            sprintf(scoreText, "Score: %d", score);
            displayMessage(scoreText, rgb565(255, 255, 255));

            _delay_ms(2000);
            *state = IDLE; // Reset the game
            break;
        case IDLE:
            ;
    }
    if(ball->score >= 5){
        reset(ball, hitter);
        LCD_drawString(5, 43, "Congratulation!", rgb565(255, 165, 0), rgb565(0, 0, 0));
        LCD_drawString(10, 73, "Pitcher Win!", rgb565(255, 165, 0), rgb565(0, 0, 0));
        *state = IDLE;
    }else if(hitter->score >= 5){
        reset(ball, hitter);
        LCD_drawString(5, 43, "Congratulation!", rgb565(255, 165, 0), rgb565(0, 0, 0));
        LCD_drawString(10, 73, "Hitter Win!", rgb565(255, 165, 0), rgb565(0, 0, 0));
        *state = IDLE;
    }
    _delay_ms(1000); // Prevent rapid state transitions
    
//    LCD_setScreen(rgb565(0, 0, 0));
}
