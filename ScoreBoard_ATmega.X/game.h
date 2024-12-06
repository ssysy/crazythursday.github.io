/* 
 * File:   game.h
 * Author: xiezhuozi
 *
 * Created on November 29, 2024, 6:06 PM
 */

#ifndef GAME_H
#define	GAME_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Game objects
typedef struct {
    uint8_t x, y;    // Position of the ball
    uint8_t v;
    uint8_t a;
    uint8_t radius;  // Radius of the ball
    uint16_t color;  // Color of the ball
    bool moving;     // Is the ball moving?
    uint8_t score;
    bool dispal;
} Ball;

typedef struct {
    uint8_t x, y;    // Position of the catcher
    uint8_t width, height; // Dimensions of the catcher
    uint16_t color;  // Color of the catcher
    uint8_t score;
} Catcher;



typedef struct {
    uint8_t x, y;    // Position of the hitter piston
    uint8_t width, height; // Dimensions of the piston
    uint16_t color;  // Color of the piston
    uint8_t score;
} Hitter;

typedef enum { PREPARE, PITCHING, HIT, CATCHER, IDLE } GameState;
void drawBall(Ball* ball);
void clearBall(Ball* ball);
void drawCatcher(Catcher* catcher);
void clearCatcher(Catcher* catcher);
void drawHitter(Hitter* hitter);
void clearHitter(Hitter* hitter);
void displayMessage(char* message, uint16_t color);
bool moveBallUp(Ball* ball);
void drawscore(Ball* ball, Hitter* hitter);
void game(Ball* ball, Catcher* catcher, Hitter* hitter, GameState* state, unsigned char* received_data);



#ifdef	__cplusplus
}
#endif

#endif	/* GAME_H */

