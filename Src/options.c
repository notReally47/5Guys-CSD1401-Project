#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "structs.h"
#include "options_draw.h"
#include <stdio.h> // sprintf_s()
#include <stdlib.h> //_countof()
#include <string.h> // strlen() & strcpy_s()

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
int ddlClicked, isWindowed, configChanged;

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
	back.name = "<-"; // to be replaced with an image

	currentRes.actWidth = config.settings.resolutionWidth, currentRes.actHeight = config.settings.resolutionHeight;
	currentRes.windowed = config.settings.windowed;

	halfscreenWindowed.actHeight = (unsigned int)(CP_System_GetDisplayHeight() / 2);
	halfscreenWindowed.actWidth = (unsigned int)(CP_System_GetDisplayWidth() / 2);
	halfscreenWindowed.windowed = 1, halfscreenWindowed.selected = 0;

	fullscreenWindowed.actHeight = (unsigned int)(CP_System_GetDisplayHeight());
	fullscreenWindowed.actWidth = (unsigned int)(CP_System_GetDisplayWidth());
	fullscreenWindowed.windowed = 1, fullscreenWindowed.selected = 0;

	fullscreen.actHeight = (unsigned int)(CP_System_GetDisplayHeight());
	fullscreen.actWidth = (unsigned int)(CP_System_GetDisplayWidth());
	fullscreen.windowed = 0, fullscreen.selected = 0;

	volumeDown.name = "-";
	volumeUp.name = "+";

	apply.name = "Apply";
	discard.name = "Discard";

	// Dynamic struct fields
	back.btnWidth = strlen(back.name) * textSize / 2, back.btnHeight = back.btnWidth;
	back.position = CP_Vector_Set(back.btnWidth / 2.0f + PADDING, back.btnHeight / 2.0f + PADDING);

	currentRes.position = CP_Vector_Set(window.x - back.btnWidth * 2.5 - 1.5 * PADDING, back.btnHeight + 3.5 * PADDING);
	currentRes.btnWidth = 15 * textSize, currentRes.btnHeight = back.btnHeight;

	halfscreenWindowed.btnWidth = currentRes.btnWidth, halfscreenWindowed.btnHeight = currentRes.btnHeight;
	halfscreenWindowed.position = CP_Vector_Set(currentRes.position.x, currentRes.position.y + halfscreenWindowed.btnHeight);

	fullscreenWindowed.btnWidth = currentRes.btnWidth, fullscreenWindowed.btnHeight = currentRes.btnHeight;
	fullscreenWindowed.position = CP_Vector_Set(halfscreenWindowed.position.x, halfscreenWindowed.position.y + fullscreenWindowed.btnHeight);

	fullscreen.btnWidth = currentRes.btnWidth, fullscreen.btnHeight = currentRes.btnHeight;
	fullscreen.position = CP_Vector_Set(fullscreenWindowed.position.x, fullscreenWindowed.position.y + fullscreen.btnHeight);

	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;

	volumeDown.btnWidth = back.btnWidth, volumeDown.btnHeight = back.btnWidth;
	volumeUp.btnWidth = back.btnWidth, volumeUp.btnHeight = back.btnWidth;

	apply.btnWidth = strlen(apply.name) * textSize / 2, apply.btnHeight = back.btnHeight + PADDING;
	apply.position = CP_Vector_Set(window.x / 2 - 5 * PADDING - apply.btnWidth / 2, window.y - PADDING - apply.btnHeight / 2);

	discard.btnWidth = strlen(discard.name) * textSize / 2, discard.btnHeight = back.btnHeight + PADDING;
	discard.position = CP_Vector_Set(window.x / 2 + 5 * PADDING + discard.btnWidth / 2, window.y - PADDING - discard.btnHeight / 2);

	changes[0] = apply, changes[1] = discard;

	// Gif
	imageIndex = 0;
	timeElapsed = 0.0f;

	// Dropdown-list
	ddlClicked = 0;
	isWindowed = 1;
	configChanged = 0;
	
	for (int i = 0; i < 3; i++) {
		if (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed)
		{
			resolution[i].selected = 1;
			resSelected = &resolution[i];
		}
	}
}

void Options_Update() {
	CP_Vector mouse = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()); // mouse position
	CP_Color currentResColor = ddlClicked ? DARKGRAY : GRAY;
	window = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	textSize = (float)window.y * 0.05f;
	CP_Settings_NoTint();
	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_TextSize(textSize);

	for (int i = 0; i < 3; i++)
	{
		if (resolution[i].selected) {
			resSelected = &resolution[i];
			printf("Res no. : %d\n", i);
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
	drawTintedButton(RED, back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y, 0);
	drawCenterAlignedText(BLACK, back.name, back.position.x, back.position.y);

	/*Draw headers: Resolution, Volume, Tutorial*/
	drawLeftAlignedText(BLACK, "Resolution", PADDING, back.btnHeight + 2 * PADDING);
	drawLeftAlignedText(BLACK, "Volume", PADDING, back.btnHeight + 3 * PADDING + textSize);
	drawLeftAlignedText(BLACK, "Tutorial", PADDING, back.btnHeight + 4 * PADDING + 2 * textSize);

	char displayRes[25] = { 0 };
	resSelected->windowed ? sprintf_s(displayRes, _countof(displayRes), "%d x %d (windowed)", resSelected->actWidth, resSelected->actHeight) :
		sprintf_s(displayRes, _countof(displayRes), "%d x %d (fullscreen)", resSelected->actWidth, resSelected->actHeight);

	/*Draw current volume and the volume up and down buttons*/
	char currentVol[16] = { 0 };
	int vol = 0; // CP_Sound_GetGroupVolume() when we import sound
	sprintf_s(currentVol, _countof(currentVol), "%d", vol);
	drawRightAlignedText(BLACK, currentVol, window.x - PADDING, back.btnHeight + 3 * PADDING + textSize);

	/*Draw tutorial*/
	drawGIF(gameplay,
		window.x - PADDING - gifDimension, back.btnHeight + 4 * PADDING + 2 * textSize,
		gifDimension, gifDimension,
		DISPLAY_DURATION, FRAME_DIMENSION, timeElapsed, imageIndex, TOTAL_FRAMES, SPRITESHEET_ROWS);
	drawNonTintedButton(RED, 2 * PADDING + 75, window.y - 2 * PADDING - 75, 50, 50);
	drawNonTintedButton(RED, PADDING + 25, window.y - PADDING - 25, 50, 50);
	drawNonTintedButton(RED, 2 * PADDING + 75, window.y - PADDING - 25, 50, 50);
	drawNonTintedButton(RED, 3 * PADDING + 125, window.y - PADDING - 25, 50, 50);
	drawCenterAlignedText(BLACK, "W", 2 * PADDING + 75, window.y - 2 * PADDING - 75);
	drawCenterAlignedText(BLACK, "A", PADDING + 25, window.y - PADDING - 25);
	drawCenterAlignedText(BLACK, "S", 2 * PADDING + 75, window.y - PADDING - 25);
	drawCenterAlignedText(BLACK, "D", 3 * PADDING + 125, window.y - PADDING - 25);

	/*Draw Resolution dropdown-list*/
	drawTintedButton(currentResColor, currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y, 1);
	drawRightAlignedText(BLACK, displayRes, window.x - PADDING, back.btnHeight + 2 * PADDING);
	float textX = window.x - PADDING, textY = back.btnHeight + 2 * PADDING + currentRes.btnHeight;
	if (CP_Input_MouseClicked() && IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		for (int i = 0; i < 3; i++, textY += currentRes.btnHeight) {
			drawTintedButton(GRAY, resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y, 1);
			drawRightAlignedText(BLACK, resolution[i].name, textX, textY);
		}
		CP_Font_DrawText(displayRes, window.x - PADDING, back.btnHeight + 2 * PADDING);
		ddlClicked = 1;
	}
	if (ddlClicked) {
		for (int i = 0; i < 3; i++, textY += currentRes.btnHeight)
		{
			drawTintedButton(GRAY, resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y, 1);
			drawRightAlignedText(BLACK, resolution[i].name, textX, textY);
		}

		for (int i = 0; i < 3; i++) {
			if (CP_Input_MouseClicked()) {
				if (IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y)) {
					resolution[i].selected = 1;
					configChanged = (resolution[i].actWidth == currentRes.actWidth && resolution[i].actHeight == currentRes.actHeight && resolution[i].windowed == currentRes.windowed) ? 0 : 1;
				}
				else resolution[i].selected = 0;
			}
		}
	}

	/*Apply and Discard Changes (does not include audio yet)*/
	if (configChanged) {
		for (int i = 0; i < 2; i++)
		{
			if (CP_Input_MouseClicked()) {
				if (IsAreaClicked(changes[0].position.x, changes[0].position.y, changes[0].btnWidth, changes[0].btnHeight, mouse.x, mouse.y)) {
					newConfig.settings.resolutionWidth = resSelected->actWidth;
					newConfig.settings.resolutionHeight = resSelected->actHeight;
					newConfig.settings.windowed = resSelected->windowed;
					CP_System_SetWindowSize(newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight);
					if (!newConfig.settings.windowed) {
						CP_System_Fullscreen();
					}
					configChanged = 0;
				}
				if (IsAreaClicked(changes[1].position.x, changes[1].position.y, changes[1].btnWidth, changes[1].btnHeight, mouse.x, mouse.y)) {
					CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
					configChanged = 0;
				}
			}
			drawTintedButton(RED, changes[i].position.x, changes[i].position.y, changes[i].btnWidth, changes[i].btnHeight, mouse.x, mouse.y, 0);
			drawCenterAlignedText(BLACK, changes[i].name, changes[i].position.x, changes[i].position.y);
		}
	}
	if (CP_Input_MouseClicked() && !IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		ddlClicked = 0;
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
