#pragma once
#include "cprocessing.h"
#include "structs.h"
#include "defines.h"

/*
* setButton - Initialize Button stuct
* Button* btn: Pointer to button created
* const char* img: Filepath to image to be drawn
* float x: X-coordinate of button position
* float y: Y-coordinate of button position
* float w: Width of button
* float h: Height of button
* Flag tint: Flag to determine whether the button is tinted when mouse over
*/
void setButton(Button* btn, const char* img, float x, float y, float w, float h, Flag tint);

/*
* setDropDownList - Initialize DropDownList struct
* DropDownList* ddl: Pointer to dropdownlist created
* unsigned int w: Resolution width
* unsigned int h: Resolution height
* Flag windowed: Flag to determine whether the resolution is windowed
* float x: X-coordinate of dropdownlist position
* float y: Y-coordinate of dropdownlist position
* float btnWidth: Width of button
* float btnHeight: Height of button
*/
void setDropDownList(DropDownList* ddl, unsigned int w, unsigned int h, Flag windowed, float x, float y, float btnWidth, float btnHeight);

/*
* setGIF - Initialize GIF struct
* GIF* gif: Pointer to gif created
* const char* spritesheet: Spritesheet to be animated
* int rows: Number of rows in the spritesheet
* int cols: Number of columns in the spritesheet
* float x: X-coordinate of gif position
* float y: Y-coordinate of gif position
* float dimensions: Dimensions of gif drawn in the game (square)
*/
void setGIF(GIF* gif, const char* spritesheet, int rows, int cols, float x, float y, float dimensions);

/*
* drawTintedButton - Draw a button that casts a tint on the button when mouse over
* CP_Color color: Color of button
* float x: X-coordinate of button position
* float y: Y-coordinate of button position
* float w: Width of button
* float h: Height of button
* float mouse_x: X-coordinate of mouse position
* float mouse_y: Y-coordinate fo mouse position
* Flag isDDL: Flag to determine whether it is a dropdownlist or not
*/
void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, Flag isDDL);

/*
* drawAlignedText - Specify the alignment of text drawn
* CP_Color color: Color of text
* int alignment: Alignment of text
* const char* text: Text to be drawn
* float x: X-coordinate of text position
* float y: Y-coordinate fo text position
*/
void drawAlignedText(CP_Color color, int alignment, const char* text, float x, float y);

/*
* drawGIF - Draw GIF
* GIF* gif: Pointer of gif created
* float* timeElapsed: Pointer to time elapsed in game
* const float displayDuration: Duration each frame to be displayed in seconds
* Flag flip: Flag to determine whether to flip the left and right of the gif
*/
void drawGIF(GIF* gif, float* timeElapsed, const float displayDuration, Flag flip);

/*
* drawButton - Draws button from button struct
* Button btn - Button to be drawn
*/
void drawButton(Button btn);

/*
* drawHeader - Draws headers
* const char* stringArr[]: List of headers to be drawn
* int size: Number of elements in array
*/
void drawHeader(const char* stringArr[], int size);

/*
* freeButtonImg - Free images stores in button struct from memory
* Button btn[]: Array of buttons
* int size: Number of elements in array
*/
void freeButtonImg(Button btn[], int size);
