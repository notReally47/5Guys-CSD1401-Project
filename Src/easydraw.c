#include "cprocessing.h"
#include "utils.h"
#include "structs.h"
#include "defines.h"

Button controls[9], up, down, left, right, pause, undo, reset, escape, camera;
float imgSize, textSize;
CP_Image arrowDown;

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
	gif->imgWidth = (float)CP_Image_GetWidth(gif->spritesheet);
	gif->imgHeight = (float)CP_Image_GetHeight(gif->spritesheet);
	gif->gifDimensions = dimensions;
	gif->numCols = cols;
	gif->numRows = rows;
	gif->currCol = 0;
	gif->imgIndex = 0;
}

void drawTintedButton(CP_Color color, float x, float y, float w, float h, float mouse_x, float mouse_y, Flag isDDL) {
	IsAreaClicked(x, y, w, h, mouse_x, mouse_y) ? isDDL ? CP_Settings_Tint(BGDBLUET) : CP_Settings_Tint(DARKGRAY) : CP_Settings_NoTint();
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(0.0f);
	CP_Graphics_DrawRect(x, y, w, h);
	CP_Settings_NoTint();
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

void drawGIF(GIF* gif, float* timeElapsed, const float displayDuration, Flag flipLR, Flag flipTB) {
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

	left = flipLR ? ((gif->imgIndex % gif->numCols) + 1) * frameWidth : (gif->imgIndex % gif->numCols) * frameWidth;
	top = flipTB ? (gif->currCol + 1) * frameHeight : gif->currCol * frameHeight;
	right = flipLR ? (gif->imgIndex % gif->numCols) * frameWidth : ((gif->imgIndex % gif->numCols) + 1) * frameWidth;
	bottom = flipTB ? gif->currCol * frameHeight : (gif->currCol + 1) * frameHeight;

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
		drawAlignedText(FADERBLACK, LEFT, stringArr[i], textX, textY);
		drawAlignedText(BLACK, LEFT, stringArr[i], textX + 2, textY);
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

void initControls() {
	imgSize = (float)CP_System_GetWindowHeight() / 30.f, textSize = (float)CP_System_GetWindowHeight() / 30.f;
	setButton(&up, "./Assets/UI/W.png", (float)CP_System_GetWindowHeight() / 40.f, (float)CP_System_GetWindowHeight() / 40.f, imgSize, imgSize, NO);
	setButton(&left, "./Assets/UI/A.png", up.position.x, up.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&down, "./Assets/UI/S.png", left.position.x, left.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&right, "./Assets/UI/D.png", down.position.x, down.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&pause, "./Assets/UI/P.png", right.position.x, right.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&escape, "./Assets/UI/ESC.png", pause.position.x + imgSize, pause.position.y, imgSize, imgSize, NO);
	setButton(&undo, "./Assets/UI/U.png", pause.position.x, pause.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&reset, "./Assets/UI/R.png", undo.position.x, undo.position.y + 1.5 * imgSize, imgSize, imgSize, NO);
	setButton(&camera, "./Assets/UI/C.png", reset.position.x, reset.position.y + 1.5 * imgSize, imgSize, imgSize, NO);

	controls[0] = up;
	controls[1] = left;
	controls[2] = down;
	controls[3] = right;
	controls[4] = pause;
	controls[5] = escape;
	controls[6] = undo;
	controls[7] = reset;
	controls[8] = camera;
}

void drawControls() {
	const char* controlDescription[sizeof(controls) / sizeof(Button)];
	controlDescription[0] = "- Move up";
	controlDescription[1] = "- Move left";
	controlDescription[2] = "- Move down";
	controlDescription[3] = "- Move right";
	controlDescription[4] = "";
	controlDescription[5] = "- Pause game";
	controlDescription[6] = "- Undo move";
	controlDescription[7] = "- Reset map";
	controlDescription[8] = "- Zoom in/out";

	for (int i = 0, x = imgSize, y = imgSize / 2; i < sizeof(controls) / sizeof(Button); i++) {
		CP_Image_Draw(controls[i].img, controls[i].position.x, controls[i].position.y, controls[i].btnWidth, controls[i].btnHeight, 255);
		drawAlignedText(BLACK, LEFT, controlDescription[i], controls[i].position.x + x, controls[i].position.y);
	}
}

void freeControls() {
	freeButtonImg(controls, 9);
}

void initArrow() {
	arrowDown = CP_Image_Load("./Assets/UI/arrowDown.png");
}

void drawArrow(float cellSize, float cellAlign, int playerPosX, int playerPosY) {
	CP_Image_Draw(arrowDown, cellSize * (float)playerPosY + cellAlign, cellSize * (float)playerPosX + cellSize * 0.85f, cellSize, cellSize, 255);
}

void freeArrow() {
	CP_Image_Free(&arrowDown);
}