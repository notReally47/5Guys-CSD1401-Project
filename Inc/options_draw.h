#pragma once
#include "cprocessing.h"

void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, int isDDL);
void drawNonTintedButton(CP_Color color, float x, float y, float w, float h);
void drawCenterAlignedText(CP_Color color, const char* text, float x, float y);
void drawLeftAlignedText(CP_Color color, const char* text, float x, float y);
void drawRightAlignedText(CP_Color color, const char* text, float x, float y);
void drawGIF(CP_Image img, float x, float y, float w, float h, static const float display_duration, static const float frame_dimension, static float timeElapsed, static int imageIndex, const int totalFrames, const int numRows);
