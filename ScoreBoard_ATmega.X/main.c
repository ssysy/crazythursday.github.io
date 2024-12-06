/*
 * File:   main.c
 * Author: xiezhuozi
 *
 * Created on November 27, 2024, 7:28 PM
 */


//#include <xc.h>
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h> 
#include <stdbool.h>
#include "ST7735.h"
#include "LCD_GFX.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

// Game states
typedef enum { PREPARE, PITCHING, HIT, CATCHER } GameState;

// Game objects
typedef struct {
    uint8_t x, y;    // Position of the ball
    uint8_t radius;  // Radius of the ball
    uint16_t color;  // Color of the ball
    bool moving;     // Is the ball moving?
} Ball;

typedef struct {
    uint8_t x, y;    // Position of the catcher
    uint8_t width, height; // Dimensions of the catcher
    uint16_t color;  // Color of the catcher
} Catcher;

typedef struct {
    uint8_t x, y;    // Position of the hitter piston
    uint8_t width, height; // Dimensions of the piston
    uint16_t color;  // Color of the piston
} Hitter;

// Initialize game objects
void initializeGame(Ball* ball, Catcher* catcher, Hitter* hitter) {
    lcd_init();
    LCD_setScreen(rgb565(0, 0, 0)); // Set screen to black

    // Initialize the ball
    ball->x = 80; // Centered horizontally
    ball->y = 120; // Bottom of the screen
    ball->radius = 5;
    ball->color = rgb565(255, 255, 255); // White ball
    ball->moving = false;

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
}

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
    LCD_setScreen(rgb565(0, 0, 0));
    LCD_drawString(10, 60, message, color, rgb565(0, 0, 0));
}

// Simulate the ball movement
bool moveBallUp(Ball* ball, Hitter* hitter) {
    clearBall(ball);
    clearHitter(hitter);

    if (ball->y > 0) {
        ball->y -= 5; // Move ball upward
        drawBall(ball);

        // Update hitter position
        hitter->y = ball->y + 10;
        drawHitter(hitter);

        return true; // Ball is still in motion
    }

    return false; // Ball reached the top
}

int main(void) {
    Ball ball;
    Catcher catcher;
    Hitter hitter;
    GameState state = PREPARE;
    uint8_t score = 0;

    initializeGame(&ball, &catcher, &hitter);

    while (1) {
        switch (state) {
            case PREPARE:
                displayMessage("Pitching!", rgb565(255, 255, 255));
                ball.y = 120; // Reset ball position at the bottom
                hitter.y = ball.y + 10; // Reset hitter position
                ball.moving = false;

                // Simulate input to start pitching
                if (/* Replace with input check */ true) {
                    ball.moving = true;
                    state = PITCHING;
                    clearBall(&ball);
                    clearHitter(&hitter);
                    displayMessage("", rgb565(0, 0, 0)); // Clear message
                }
                break;

            case PITCHING:
                if (ball.moving) {
                    bool stillMoving = moveBallUp(&ball, &hitter);
                    if (!stillMoving) {
                        state = HIT; // Switch to HIT state for external input
                        ball.moving = false;
                    }
                }
                break;

            case HIT:
                displayMessage("Did the player hit?", rgb565(255, 255, 0));
                _delay_ms(1000); // Simulate a delay to read input

                // Check external input for a "hit"
                if (/* Replace with input check for hit */ false) {
                    displayMessage("Hit!", rgb565(0, 255, 0)); // Green text for hit
                    _delay_ms(2000);
                    score++; // Increment score for a successful hit
                    state = PREPARE; // Reset the game
                } else {
                    displayMessage("No hit!", rgb565(255, 255, 0)); // Yellow text for no hit
                    _delay_ms(2000);
                    state = CATCHER; // Transition to catcher logic
                }
                break;

            case CATCHER:
                drawCatcher(&catcher);
                if (ball.x >= catcher.x && ball.x <= catcher.x + catcher.width &&
                    ball.y <= catcher.y + catcher.height) {
                    displayMessage("Caught!", rgb565(0, 255, 0)); // Green text for caught
                    score++; // Increment score for a successful catch
                } else {
                    displayMessage("Missed!", rgb565(255, 0, 0)); // Red text for missed
                }

                char scoreText[16];
                sprintf(scoreText, "Score: %d", score);
                displayMessage(scoreText, rgb565(255, 255, 255));

                _delay_ms(2000);
                state = PREPARE; // Reset the game
                break;
        }
        _delay_ms(50);
    }
    return 0;
}
