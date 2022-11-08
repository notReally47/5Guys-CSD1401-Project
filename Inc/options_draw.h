#pragma once
#include "cprocessing.h"

void setButton(Button* btn, const char* img, float x, float y, float w, float h, int tint);
void setDropDownList(DropDownList* ddl, unsigned int w, unsigned int h, int windowed, float x, float y, float btnWidth, float btnHeight);
void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, int isDDL);
void drawAlignedText(CP_Color color, int alignment, const char* text, float x, float y);
void drawGIF(CP_Image img, float x, float y, float w, float h, static const float display_duration, static const float frame_dimension, static float timeElapsed, static int imageIndex, const int totalFrames, const int numRows);
void drawButton(Button btn);
void drawHeader(const char* stringArr[], int size);
void freeButtonImg(Button btn[], int size);
