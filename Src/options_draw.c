#include "cprocessing.h"
#include "utils.h"
#include "structs.h"
#include "defines.h"

void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, int isDDL) {
	float strokeWeight = isDDL ? 0.0f : 3.0f;
	IsAreaClicked(x, y, w, h, mouse_x, mouse_y) ? CP_Settings_Tint(DARKGRAY) : CP_Settings_NoTint();
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(strokeWeight);
	CP_Graphics_DrawRect(x, y, w, h);
}

void drawNonTintedButton(CP_Color color, float x, float y, float w, float h) {
	CP_Settings_NoTint();
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(3.0f);
	CP_Graphics_DrawRect(x, y, w, h);
}

void drawAlignedText(CP_Color color, int alignment, const char *text, float x, float y) {
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

void drawGIF(CP_Image img, float x, float y, float w, float h, static const float display_duration, static const float frame_dimension, static float timeElapsed, static int imageIndex, int totalFrames, int numRows) {
	CP_Settings_NoTint();
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_DrawSubImage(img, x, y, w, h,
		(imageIndex % (totalFrames / numRows))* frame_dimension, (imageIndex < (totalFrames / numRows)) ? 0 : frame_dimension,
		((imageIndex % (totalFrames / numRows)) + 1)* frame_dimension, (imageIndex < (totalFrames / numRows)) ? frame_dimension : frame_dimension * numRows,
		255);
}
