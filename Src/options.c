#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "structs.h"
#include <stdlib.h> //_countof()
#include <stdio.h> // sprintf_s()

Button back, resolution[3], currentRes, halfscreenWindowed, fullscreenWindowed, fullscreen, volumeDown, volumeUp;
float textSize, volume, numCols, numRows;

// Gif
static float timeElapsed;
static const float DISPLAY_DURATION = .5f;
static int imageIndex;
static const float FRAME_DIMENSION = 600.0f;

CP_Vector window;
CP_Image gameplay;
int ddlClicked, isWindowed;

extern Config config;
Config newConfig;

void Options_Init() {
	/*Initialize config*/
	newConfig = config;
	CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);

	textSize = (float)CP_System_GetWindowHeight() * 0.05f;
	CP_Settings_TextSize(textSize);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

	window = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	gameplay = CP_Image_Load("./Assets/Gameplay.png");

	// Back button
	back.text = "<-"; // to be replaced with an image
	back.btnWidth = CP_System_GetWindowWidth() / 40.0f, back.btnHeight = back.btnWidth;
	back.position = CP_Vector_Set(back.btnWidth / 2.0f + PADDING, back.btnHeight / 2.0f + PADDING);

	// Resolution selections
	currentRes.btnWidth = 15 * textSize, currentRes.btnHeight = back.btnHeight;
	currentRes.position = CP_Vector_Set(window.x - back.btnWidth * 2.5 - 1.5 * PADDING, back.btnHeight + 3.5 * PADDING, back.btnWidth);

	halfscreenWindowed.btnWidth = currentRes.btnWidth, halfscreenWindowed.btnHeight = currentRes.btnHeight;
	halfscreenWindowed.position = CP_Vector_Set(currentRes.position.x, currentRes.position.y + halfscreenWindowed.btnHeight);
	halfscreenWindowed.windowed = 1;
	halfscreenWindowed.actHeight = CP_System_GetDisplayHeight() / 2;
	halfscreenWindowed.actWidth = CP_System_GetDisplayWidth() / 2;

	fullscreenWindowed.btnWidth = currentRes.btnWidth, fullscreenWindowed.btnHeight = currentRes.btnHeight;
	fullscreenWindowed.position = CP_Vector_Set(halfscreenWindowed.position.x, halfscreenWindowed.position.y + fullscreenWindowed.btnHeight);
	fullscreenWindowed.windowed = 1;
	fullscreenWindowed.actHeight = CP_System_GetDisplayHeight();
	fullscreenWindowed.actWidth = CP_System_GetDisplayWidth();

	fullscreen.btnWidth = currentRes.btnWidth, fullscreen.btnHeight = currentRes.btnHeight;
	fullscreen.position = CP_Vector_Set(fullscreenWindowed.position.x, fullscreenWindowed.position.y + fullscreen.btnHeight);
	fullscreen.windowed = 0;
	fullscreen.actHeight = CP_System_GetDisplayHeight();
	fullscreen.actWidth = CP_System_GetDisplayWidth();

	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;


	// Volume up and down buttons
	volumeDown.text = "-";
	volumeDown.btnWidth = back.btnWidth, volumeDown.btnHeight = back.btnWidth;
	volumeUp.text = "+";
	volumeUp.btnWidth = back.btnWidth, volumeUp.btnHeight = back.btnWidth;

	// Gif
	imageIndex = 0;
	timeElapsed = 0.0f;
	ddlClicked = 0;
	isWindowed = 1;
}

void Options_Update() {
	CP_Vector mouse = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()); // mouse position

	char halfscreenWRes[25] = { 0 };
	sprintf_s(halfscreenWRes, _countof(halfscreenWRes), "%d x %d (windowed)", CP_System_GetDisplayWidth() / 2, CP_System_GetDisplayHeight() / 2);
	char fullscreenWRes[25] = { 0 };
	sprintf_s(fullscreenWRes, _countof(fullscreenWRes), "%d x %d (windowed)", CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
	char fullscreenRes[25] = { 0 };
	sprintf_s(fullscreenRes, _countof(fullscreenRes), "%d x %d (fullscreen)", CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());

	// Draw gameplay gif logic
	timeElapsed += CP_System_GetDt();
	if (timeElapsed >= DISPLAY_DURATION) {
		imageIndex = (imageIndex + 1) % 14;
		timeElapsed = 0.0f;
	}
	CP_Graphics_ClearBackground(GRAY);

	// Highlight button when mouse over
	if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y)) {
		CP_Settings_Tint(DARKGRAY);
	}

	// Return to main menu when click on back button
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	// Draw back button
	CP_Settings_Fill(RED);
	CP_Graphics_DrawRect(back.position.x, back.position.y, back.btnWidth, back.btnHeight);

	// Draw Resolution dropdown-list
	if (IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		CP_Settings_Tint(DARKGRAY);
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight);
	}

	if (CP_Input_MouseClicked() && IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		CP_Settings_NoTint();
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight);
		for (int i = 0; i < 3; i++) {
			CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight);
		}
		CP_Settings_Fill(BLACK);
		CP_Settings_StrokeWeight(3.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(halfscreenWRes, window.x - PADDING, back.btnHeight + 2 * PADDING + currentRes.btnHeight);
		CP_Font_DrawText(fullscreenWRes, window.x - PADDING, back.btnHeight + 2 * PADDING + 2 * currentRes.btnHeight);
		CP_Font_DrawText(fullscreenRes, window.x - PADDING, back.btnHeight + 2 * PADDING + 3 * currentRes.btnHeight);
		ddlClicked = 1;
	}

	if (ddlClicked) {
		CP_Settings_NoTint();
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight);
		for (int i = 0; i < 3; i++)
		{
			if (IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y)) {
				CP_Settings_Tint(DARKGRAY);
				CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight);
			}
			else {
				CP_Settings_NoTint();
				CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight);
			}
		}
		CP_Settings_Fill(BLACK);
		CP_Settings_StrokeWeight(3.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(halfscreenWRes, window.x - PADDING, back.btnHeight + 2 * PADDING + currentRes.btnHeight);
		CP_Font_DrawText(fullscreenWRes, window.x - PADDING, back.btnHeight + 2 * PADDING + 2 * currentRes.btnHeight);
		CP_Font_DrawText(fullscreenRes, window.x - PADDING, back.btnHeight + 2 * PADDING + 3 * currentRes.btnHeight);

		for (int i = 0; i < 3; i++) {
			if (CP_Input_MouseClicked() && IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].btnWidth, resolution[i].btnHeight, mouse.x, mouse.y)) {
				// replace current resolution
				newConfig.settings.windowed = resolution[i].windowed;
				newConfig.settings.resolutionHeight = resolution[i].actHeight;
				newConfig.settings.resolutionWidth = resolution[i].actWidth;
				printf("%d\n", ddlClicked);
			}
		}
	}

	if (CP_Input_MouseClicked() && !IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.btnWidth, currentRes.btnHeight, mouse.x, mouse.y)) {
		ddlClicked = 0;
	}

	CP_Settings_Fill(BLACK);
	CP_Settings_StrokeWeight(3.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(back.text, back.position.x, back.position.y);

	// Draw headers: Resolution, Volume, Tutorial
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText("Resolution", PADDING, back.btnHeight + 2 * PADDING);
	CP_Font_DrawText("Volume", PADDING, back.btnHeight + 3 * PADDING + textSize);
	CP_Font_DrawText("Tutorial", PADDING, back.btnHeight + 4 * PADDING + 2 * textSize);

	// Draw current resolution and resolution selection
	char currentRes[25] = { 0 };
	newConfig.settings.windowed ? sprintf_s(currentRes, _countof(currentRes), "%d x %d (windowed)", newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight) :
		sprintf_s(currentRes, _countof(currentRes), "%d x %d (fullscreen)", newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText(currentRes, window.x - PADDING, back.btnHeight + 2 * PADDING);

	// Draw current volume and the volume up and down buttons
	//char currentVol[16] = { 0 };
	//int vol = 0; // CP_Sound_GetGroupVolume() when we import sound
	//sprintf_s(currentVol, _countof(currentVol), "%d", vol);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
	//CP_Font_DrawText(currentVol, window.x - PADDING, back.height + 3 * PADDING + textSize);

	// Draw tutorial
	CP_Settings_NoTint();
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	/*CP_Image_DrawSubImage(gameplay,
		window.x - PADDING - (window.y - (back.height + 5 * PADDING + 2 * textSize)), back.height + 4 * PADDING + 2 * textSize,
		window.y * 0.76f, window.y * 0.76f,
		(imageIndex % 7) * FRAME_DIMENSION, (imageIndex < 7) ? 0 : FRAME_DIMENSION,
		((imageIndex % 7) + 1) * FRAME_DIMENSION, (imageIndex < 7) ? FRAME_DIMENSION : FRAME_DIMENSION * 2,
		255);*/
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawRect(2 * PADDING + 75, window.y - 2 * PADDING - 75, 50, 50);
	CP_Graphics_DrawRect(PADDING + 25, window.y - PADDING - 25, 50, 50);
	CP_Graphics_DrawRect(2 * PADDING + 75, window.y - PADDING - 25, 50, 50);
	CP_Graphics_DrawRect(3 * PADDING + 125, window.y - PADDING - 25, 50, 50);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

	CP_Font_DrawText("W", 2 * PADDING + 75, window.y - 2 * PADDING - 75);
	CP_Font_DrawText("A", PADDING + 25, window.y - PADDING - 25);
	CP_Font_DrawText("S", 2 * PADDING + 75, window.y - PADDING - 25);
	CP_Font_DrawText("D", 3 * PADDING + 125, window.y - PADDING - 25);
}

void Options_Exit() {
	CP_Image_Free(&gameplay);
}
