#include "cprocessing.h"
#include "utils.h"
#include "structs.h"
#include "defines.h"

void setButton(Button* btn, const char* img, float x, float y, float w, float h, Flag tint) {
	btn->img = CP_Image_Load(img);
	btn->position = CP_Vector_Set(x, y);
	btn->btnWidth = w;
	btn->btnHeight = h;
	btn->tint = tint;
}

void setDropDownList(DropDownList* ddl, unsigned int w, unsigned int h, Flag windowed, float x, float y, float btnWidth, float btnHeight) {
	ddl->actWidth = w;
	ddl->actHeight = h;
	ddl->windowed = windowed;
	ddl->selected = NO;
	ddl->button.img = NULL;
	ddl->button.position = CP_Vector_Set(x, y);
	ddl->button.btnWidth = btnWidth;
	ddl->button.btnHeight = btnHeight;
	ddl->button.tint = YES;
}

void setGIF(GIF* gif, const char* spritesheet, int rows, int cols, float x, float y, float dimensions) {
	gif->spritesheet = CP_Image_Load(spritesheet);
	gif->position = CP_Vector_Set(x, y);
	gif->numCols = cols;
	gif->numRows = rows;
	gif->currCol = 0;
	gif->imgIndex = 0;
	gif->imgWidth = (float)CP_Image_GetWidth(gif->spritesheet);
	gif->imgHeight = (float)CP_Image_GetHeight(gif->spritesheet);
	gif->gifDimensions = dimensions;
}

void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, Flag isDDL) {
	float strokeWeight = isDDL ? 0.0f : 3.0f;
	IsAreaClicked(x, y, w, h, mouse_x, mouse_y) ? CP_Settings_Tint(DARKGRAY) : CP_Settings_NoTint();
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(strokeWeight);
	CP_Graphics_DrawRect(x, y, w, h);
}

void drawAlignedText(CP_Color color, int alignment, const char* text, float x, float y) {
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(3.0f);
	switch (alignment) {
	case CENTER:
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		break;
	case LEFT:
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		break;
	case RIGHT:
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		break;
	default:
		break;
	}
	CP_Font_DrawText(text, x, y);
}

void drawGIF(GIF* gif, float* timeElapsed, const float displayDuration, Flag flip) {
	int totalFrames = gif->numRows * gif->numCols;
	float frameWidth = gif->imgWidth / gif->numCols;
	float frameHeight = gif->imgHeight / gif->numRows;
	float left, top, right, bottom;

	if (*timeElapsed >= displayDuration) {
		gif->imgIndex = (gif->imgIndex + 1) % totalFrames;
		if ((gif->imgIndex % gif->numCols) == 0) {
			gif->currCol = (gif->currCol + 1) % gif->numRows;
		}
		*timeElapsed = 0.f;
	}

	left = flip ? ((gif->imgIndex % gif->numCols) + 1) * frameWidth : (gif->imgIndex % gif->numCols) * frameWidth;
	top = gif->currCol * frameHeight;
	right = flip ? (gif->imgIndex % gif->numCols) * frameWidth : ((gif->imgIndex % gif->numCols) + 1) * frameWidth;
	bottom = (gif->currCol + 1) * frameHeight;

	CP_Image_DrawSubImage(gif->spritesheet, gif->position.x, gif->position.y, gif->gifDimensions, gif->gifDimensions, left, top, right, bottom, 255);
}

void drawButton(Button btn) {
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	btn.tint ? IsAreaClicked(btn.position.x, btn.position.y, btn.btnWidth, btn.btnHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY()) ? CP_Settings_Tint(DARKGRAY) : CP_Settings_NoTint() : CP_Settings_NoTint();
	CP_Image_Draw(btn.img, btn.position.x, btn.position.y, btn.btnWidth, btn.btnHeight, 255);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_NoTint();
}

/*
* drawDivider - Draws a divider
* float y: Y-coordinate of divider position
*/
void drawDivider(float y) {
	CP_Settings_Stroke(WHITE);
	CP_Settings_StrokeWeight(3.0f);
	CP_Graphics_DrawLine(PADDING, y, CP_System_GetWindowWidth() - PADDING, y);
	CP_Settings_Stroke(BLACK);
}

void drawHeader(const char* stringArr[], int size) {
	float textX = PADDING, textSize = CP_System_GetWindowHeight() / 20, textY = textSize + 2 * PADDING;
	float divY = 2 * PADDING + textSize + textSize / 2 + 0.6 * textSize;
	for (int i = 0; i < size; i++, textY += PADDING + textSize) {
		drawAlignedText(BLACK, LEFT, stringArr[i], textX, textY);
		if (!(i % 2)) {
			drawDivider(divY);
			divY += 0.9 * textSize + 1.5 * PADDING;
		}
	}
}

void freeButtonImg(Button btn[], int size) {
	for (int i = 0; i < size; i++) {
		CP_Image_Free(&(btn[i].img));
	}
}
