/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 
#include <math.h>
#include <string.h>
#include "old/ST7735.h"
#include "LCD_GFX.h"

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}

void LCD_drawChar_size(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor, uint8_t size){
    uint16_t row = character - 0x20;  // Determine row of ASCII table starting at space
    int i, j, m, n;
    // Adjust the bounds check for scaled character size
    if ((LCD_WIDTH - x > (5 * size - 1)) && (LCD_HEIGHT - y > (8 * size - 1))){
        for(i = 0; i < 5; i++){
            uint8_t pixels = ASCII[row][i]; // Get the pixel data for the character
            for(j = 0; j < 8; j++){
                uint16_t color = ((pixels >> j) & 1) ? fColor : bColor; // Determine the color
                // Draw a block of pixels to scale the character
                for(m = 0; m < size; m++){
                    for(n = 0; n < size; n++){
                        LCD_drawPixel(x + i * size + m, y + j * size + n, color);
                    }
                }
            }
        }
    }
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
//void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
//{
//
//	if ((LCD_WIDTH-x0>7)&&(LCD_HEIGHT-y0>7)){
//        for(int i = 0; i <= radius; i++){
//            int x1 = (int)sqrt(fabs(pow(radius, 2)-pow(i, 2)));
//            int x2 = (int)sqrt(fabs(pow(radius, 2)-pow(i, 2)));
//            for(int j = x0-x2; j < x0+x1; j++){
//                LCD_drawPixel(j,y0+i,color); 
//            }
//            for(int j = x0-x2; j < x0+x1; j++){
//                LCD_drawPixel(j,y0-i,color); 
//            }
//        }
//	}
//}

void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color) {
    // Define the background color (adjust as needed)
    uint16_t backgroundColor = 0x0000; // Black

    // Calculate bounding rectangle for the circle
    int x_start = x0 - radius;
    int x_end = x0 + radius;
    int y_start = y0 - radius;
    int y_end = y0 + radius;

    // Ensure the coordinates are within the LCD boundaries
    if (x_start < 0) x_start = 0;
    if (y_start < 0) y_start = 0;
    if (x_end >= LCD_WIDTH) x_end = LCD_WIDTH - 1;
    if (y_end >= LCD_HEIGHT) y_end = LCD_HEIGHT - 1;

    // Set the address window to the bounding rectangle
    LCD_setAddr(x_start, y_start, x_end, y_end);
    clear(LCD_PORT, LCD_TFT_CS);

    // Loop through each pixel in the bounding rectangle
    for (int y = y_start; y <= y_end; y++) {
        for (int x = x_start; x <= x_end; x++) {
            int dx = x - x0;
            int dy = y - y0;
            // Check if the pixel is within the circle
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                // Pixel is inside the circle
                SPI_ControllerTx_16bit(color);
            } else {
                // Pixel is outside the circle
                SPI_ControllerTx_16bit(backgroundColor);
            }
        }
    }

    set(LCD_PORT, LCD_TFT_CS);
}


/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{
		if(x0 != x1){
        int k = (y0-y1)/(x0-x1);
        int b = y1-k*x1;
        if(x0<x1){
            for(int i=x0; i<=x1; i++ ){
                LCD_drawPixel(i,k*i+b,c);
            }
        }else{
            for(int i=x1; i<=x0; i++ ){
                LCD_drawPixel(i,k*i+b,c);
            }
        }    
    }else{
        if(y0<y1){
            for(int i=y0; i<=y1; i++ ){
                LCD_drawPixel(x0, i,c);
            }
        }else{
            for(int i=y1; i<=y0; i++ ){
                LCD_drawPixel(x0, i,c);
            }  
        }

    }

}



/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_setAddr(x0, y0, x1, y1);
    clear(LCD_PORT, LCD_TFT_CS);
    for(int i= x0; i<x1+1; i++){
        for(int j = y0; j<y1+1;j++){
               SPI_ControllerTx_16bit(color);     
        }

    }
    set(LCD_PORT, LCD_TFT_CS);
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
    LCD_setAddr(0, 0, 159, 127);
    clear(LCD_PORT, LCD_TFT_CS);
    for(int i=0; i<160; i++){
        for(int j=0; j<128; j++){
            SPI_ControllerTx_16bit(color);
        }
    }
	set(LCD_PORT, LCD_TFT_CS);

}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	for(int i = 0; i< strlen(str); i++){
        LCD_drawChar_size(x+i*5*2+8, y, str[i], fg, bg, 2);
    }
}

void LCD_displayStringSPI(uint8_t x, uint8_t y, const char* str, uint16_t fColor, uint16_t bColor) {
    uint8_t size = 1;  // Scaling factor (can be adjusted)
    int charWidth = 5 * size;
    int charHeight = 8 * size;

    // Calculate the total width of the string
    int strLength = strlen(str);
    int totalWidth = strLength * charWidth;

    // Set the address window to cover the area where the string will be displayed
    LCD_setAddr(x, y, x + totalWidth - 1, y + charHeight - 1);

    clear(LCD_PORT, LCD_TFT_CS);

    for (int row = 0; row < charHeight; row++) {
        for (int i = 0; i < strLength; i++) {
            uint8_t character = str[i] - 0x20;  // Offset in ASCII table
            for (int col = 0; col < charWidth; col++) {
                int fontRow = row / size;
                int fontCol = col / size;

                uint8_t pixels = ASCII[character][fontCol];
                uint16_t color = ((pixels >> fontRow) & 1) ? fColor : bColor;

                SPI_ControllerTx_16bit(color);
            }
        }
    }

    set(LCD_PORT, LCD_TFT_CS);
}