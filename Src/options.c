#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "structs.h"
#include "options_draw.h"
#include <stdio.h> // sprintf_s()
#include <stdlib.h> //_countof()
#include <string.h> // strlen() & strcpy_s()
#include "options.h"

Button back, volumeDown, volumeUp, changes[2], apply, discard;
DropDownList currentRes, resolution[3], halfscreenWindowed, fullscreenWindowed, fullscreen, *resSelected;
float textSize, volume, numCols, numRows;

// Gif
static float timeElapsed;
static const float DISPLAY_DURATION = .5f;
static int imageIndex;
static const float FRAME_DIMENSION = 600.0f;
static const int TOTAL_FRAMES = 14;
static const int SPRITESHEET_ROWS = 2;
static float gifDimension;

CP_Vector window;
CP_Image gameplay;
int ddlClicked, configChanged;

extern Config config;
Config newConfig;

void Options_Init() {
	/*Initialize config*/
	newConfig = config;
	CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
	window = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	textSize = (float)window.y * 0.05f;
	gameplay = CP_Image_Load("./Assets/Gameplay.png");
	CP_Settings_TextSize(textSize);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(BLACK);

	// Initialize constant struct fields
	back.name = "back"; // to be replaced with an image

	currentRes.actWidth = config.settings.resolutionWidth, currentRes.actHeight = config.settings.resolutionHeight;
	currentRes.windowed = config.settings.windowed;

	halfscreenWindowed.actHeight = (unsigned int)(CP_System_GetDisplayHeight() / 2);
	halfscreenWindowed.actWidth = (unsigned int)(CP_System_GetDisplayWidth() / 2);
	halfscreenWindowed.windowed = YES, halfscreenWindowed.selected = NO;

	fullscreenWindowed.actHeight = (unsigned int)(CP_System_GetDisplayHeight());
	fullscreenWindowed.actWidth = (unsigned int)(CP_System_GetDisplayWidth());
	fullscreenWindowed.windowed = YES, fullscreenWindowed.selected = NO;

	fullscreen.actHeight = (unsigned int)(CP_System_GetDisplayHeight());
	fullscreen.actWidth = (unsigned int)(CP_System_GetDisplayWidth());
	fullscreen.windowed = NO, fullscreen.selected = NO;

	volumeDown.name = "-";
	volumeUp.name = "+";

	apply.name = "Apply";
	discard.name = "Discard";

	// Dynamic struct fields
	back.btnWidth = strlen(back.name) * textSize / 2, back.btnHeight = textSize;
	back.position = CP_Vector_Set(back.btnWidth / 2.0f + PADDING, back.btnHeight / 2.0f + PADDING);

	currentRes.btnWidth = MAX_LENGTH * textSize / 2 + PADDING, currentRes.btnHeight = textSize;
	currentRes.position = CP_Vector_Set(window.x - currentRes.btnWidth / 2 - PADDING, 2 * PADDING + textSize + textSize / 2);

	halfscreenWindowed.btnWidth = currentRes.btnWidth, halfscreenWindowed.btnHeight = currentRes.btnHeight;
	halfscreenWindowed.position = CP_Vector_Set(currentRes.position.x, currentRes.position.y + halfscreenWindowed.btnHeight);

	fullscreenWindowed.btnWidth = currentRes.btnWidth, fullscreenWindowed.btnHeight = currentRes.btnHeight;
	fullscreenWindowed.position = CP_Vector_Set(halfscreenWindowed.position.x, halfscreenWindowed.position.y + fullscreenWindowed.btnHeight);

	fullscreen.btnWidth = currentRes.btnWidth, fullscreen.btnHeight = currentRes.btnHeight;
	fullscreen.position = CP_Vector_Set(fullscreenWindowed.position.x, fullscreenWindowed.position.y + fullscreen.btnHeight);

	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;

	volumeDown.btnWidth = back.btnWidth, volumeDown.btnHeight = back.btnWidth;
	volumeUp.btnWidth = back.btnWidth, volumeUp.btnHeight = back.btnWidth;

	apply.btnWidth = strlen(discard.name) * textSize / 2, apply.btnHeight = back.btnHeight + PADDING;
	apply.position = CP_Vector_Set(window.x / 2 - 5 * PADDING - apply.btnWidth / 2, window.y - PADDING - apply.btnHeight / 2);

	discard.btnWidth = strlen(discard.name) * textSize / 2, discard.btnHeight = back.btnHeight + PADDING;
	discard.position = CP_Vector_Set(window.x / 2 + 5 * PADDING + discard.btnWidth / 2, window.y - PADDING - discard.btnHeight / 2);

	changes[0] = apply, changes[1] = discard;

	// Gif
	imageIndex = 0;
	timeElapsed = 0.0f;

	// Dropdown-list
	ddlClicked = NO;
	configChanged = NO;
	
	for (int i = 0; i < 3; i++) {
		resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? 1 : 0;
	}
}

void Options_Update() {
	CP_Vector mouse = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()); // mouse position
	CP_Color currentResColor = ddlClicked ? DARKGRAY : GRAY;
	CP_Settings_NoTint();
	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_TextSize(textSize);

	for (int i = 0; i < 3; i++)
	{
		if (resolution[i].selected) {
			resSelected = &resolution[i];
			//printf("Res no. : %d\n", i);
		}
	}

	const char *a[3];
	char halfscreenWRes[25] = { 0 };
	sprintf_s(halfscreenWRes, _countof(halfscreenWRes), "%u x %u (windowed)", halfscreenWindowed.actWidth, halfscreenWindowed.actHeight);
	char fullscreenWRes[25] = { 0 };
	sprintf_s(fullscreenWRes, _countof(fullscreenWRes), "%u x %u (windowed)", fullscreenWindowed.actWidth, fullscreenWindowed.actHeight);
	char fullscreenRes[25] = { 0 };
	sprintf_s(fullscreenRes, _countof(fullscreenRes), "%u x %u (fullscreen)", fullscreen.actWidth, fullscreen.actHeight);
	a[0] = halfscreenWRes, a[1] = fullscreenWRes, a[2] = fullscreenRes;
	for (int i = 0; i < 3; i++)
	{
		strcpy_s(resolution[i].name, 25, a[i]);
	}

	/*Draw gameplay gif logic*/
	timeElapsed += CP_System_GetDt();
	if (timeElapsed >= DISPLAY_DURATION) {
		imageIndex = (imageIndex + 1) % TOTAL_FRAMES;
		timeElapsed = 0.0f;
	}
	gifDimension = window.y * .6f;

	/*Draw back button*/
	drawTintedButton(RED, back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y, NO);
	drawAlignedText(BLACK, CENTER, back.name, back.position.x, back.position.y);

	/*Draw headers: Resolution, Volume, Tutorial*/
	drawAlignedText(BLACK, LEFT, "Resolution", PADDING, back.btnHeight + 2 * PADDING);
	drawAlignedText(BLACK, LEFT, "Volume", PADDING, back.btnHeight + 3 * PADDING + textSize);
	drawAlignedText(BLACK, LEFT, "Tutorial", PADDING, back.btnHeight + 4 * PADDING + 2 * textSize);

	char displayRes[25] = { 0 };
	resSelected->windowed ? sprintf_s(displayRes, _countof(displayRes), "%d x %d (windowed)", resSelected->actWidth, resSelected->actHeight) :
		sprintf_s(displayRes, _countof(displayRes), "%d x %d (fullscreen)", resSelected->actWidth, resSelected->actHeight);

	/*Draw current volume and the volume up and down buttons*/
	char currentVol[16] = { 0 };
	int vol = 0; // CP_Sound_GetGroupVolume() when we import sound
	sprintf_s(currentVol, _countof(currentVol), "%d", vol);
	drawAlignedText(BLACK, RIGHT, currentVol, window.x - PADDING, back.btnHeight + 3 * PADDING + textSize);

	/*Draw tutorial*/
	drawGIF(gameplay,
		window.x - PADDING - gifDimension, back.btnHeight + 4 * PADDING + 2 * textSize,
		gifDimension, gifDimension,
		DISPLAY_DURATION, FRAME_DIMENSION, timeElapsed, imageIndex, TOTAL_FRAMES, SPRITESHEET_ROWS);
	drawNonTintedButton(RED, 2 * PADDING + 75, window.y - 2 * PADDING - 75, 50, 50);
	drawNonTintedButton(RED, PADDING + 25, window.y - PADDING - 25, 50, 50);
	drawNonTintedButton(RED, 2 * PADDING + 75, window.y - PADDING - 25, 50, 50);
	drawNonTintedButton(RED, 3 * PADDING + 125, window.y - PADDING - 25, 50, 50);
	drawAlignedText(BLACK, CENTER, "W", 2 * PADDING + 75, window.y - 2 * PADDING - 75);
	drawAlignedText(BLACK, CENTER, "A", PADDING + 25, window.y - PADDING - 25);
	drawAlignedText(BLACK, CENTER, "S", 2 * PADDING + 75, window.y - PADDING - 25);
	drawAlignedText(BLACK, CENTER, "D", 3 * PADDING + 125, window.y - PADDING - 25);

	/*Draw Resolution dropdown-list*/
	drawTintedButton(currentResColor, currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y, YES);
	drawAlignedText(BLACK, RIGHT, displayRes, window.x - 2 * PADDING, back.btnHeight + 2 * PADDING);
	float textX = window.x - 2 * PADDING, textY = back.btnHeight + 2 * PADDING + currentRes.btnHeight;
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
				for (int i = 0; i < 3; i++, textY += currentRes.btnHeight) {
				drawTintedButton(GRAY, resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y, YES);
				drawAlignedText(BLACK, RIGHT, resolution[i].name, textX, textY);
			}
			CP_Font_DrawText(displayRes, window.x - 2 * PADDING, back.btnHeight + 2 * PADDING);
			ddlClicked = YES;
		}
	}
	textX = window.x - 2 * PADDING, textY = back.btnHeight + 2 * PADDING + currentRes.btnHeight;
	if (ddlClicked) {
		for (int i = 0; i < 3; i++, textY += currentRes.btnHeight)
		{
			drawTintedButton(GRAY, resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y, YES);
			drawAlignedText(BLACK, RIGHT, resolution[i].name, textX, textY);
		}

		for (int i = 0; i < 3; i++) {
			if (CP_Input_MouseClicked()) {
				if (IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y)) {
					resolution[i].selected = YES;
					configChanged = (resolution[i].actWidth == currentRes.actWidth && resolution[i].actHeight == currentRes.actHeight && resolution[i].windowed == currentRes.windowed) ? NO : YES;
				}
				else resolution[i].selected = NO;
			}
		}
	}

	/*Apply and Discard Changes (does not include audio yet)*/
	if (configChanged) {
		for (int i = 0; i < 2; i++)
		{
			if (CP_Input_MouseClicked()) {
				if (IsAreaClicked(changes[APPLY].position.x, changes[APPLY].position.y, changes[APPLY].btnWidth, changes[APPLY].btnHeight, mouse.x, mouse.y)) {
					newConfig.settings.resolutionWidth = resSelected->actWidth;
					newConfig.settings.resolutionHeight = resSelected->actHeight;
					newConfig.settings.windowed = resSelected->windowed;
					CP_System_SetWindowSize(newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight);
					if (!newConfig.settings.windowed) {
						CP_System_Fullscreen();
					}
					configChanged = NO;
					CP_Engine_SetNextGameStateForced(Options_Init, Options_Update, Options_Exit);
				}
				if (IsAreaClicked(changes[DISCARD].position.x, changes[DISCARD].position.y, changes[DISCARD].btnWidth, changes[DISCARD].btnHeight, mouse.x, mouse.y)) {
					for (int i = 0; i < 3; i++) {
						resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? 1 : 0;
					}
					configChanged = NO;
				}
			}
			drawTintedButton(RED, changes[i].position.x, changes[i].position.y, changes[i].btnWidth, changes[i].btnHeight, mouse.x, mouse.y, NO);
			drawAlignedText(BLACK, CENTER, changes[i].name, changes[i].position.x, changes[i].position.y);
		}
	}
	if (CP_Input_MouseClicked() && !IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		ddlClicked = NO;
	}

	/*Return to main menu when click on back button*/
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	/*Update config to newConfig*/
	config = newConfig;
}

void Options_Exit() {
	CP_Image_Free(&gameplay);
}
