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

void Options_Init() {
	textSize = (float)CP_System_GetWindowHeight() * 0.05f;
	CP_Settings_TextSize(textSize);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

	window = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	gameplay = CP_Image_Load("./Assets/Gameplay.png");

	// Back button
	back.text = "<-"; // to be replaced with an image
	back.width = CP_System_GetWindowWidth() / 40.0f, back.height = back.width;
	back.position = CP_Vector_Set(back.width / 2.0f + PADDING, back.height / 2.0f + PADDING);

	// Resolution selections
	currentRes.width = 15 * textSize, currentRes.height = back.height;
	currentRes.position = CP_Vector_Set(window.x - back.width * 2.5 - 1.5 * PADDING, back.height + 3.5 * PADDING, back.width);
	halfscreenWindowed.width = currentRes.width, halfscreenWindowed.height = currentRes.height;
	halfscreenWindowed.position = CP_Vector_Set(currentRes.position.x, currentRes.position.y + halfscreenWindowed.height);
	fullscreenWindowed.width = currentRes.width, fullscreenWindowed.height = currentRes.height;
	fullscreenWindowed.position = CP_Vector_Set(halfscreenWindowed.position.x, halfscreenWindowed.position.y + fullscreenWindowed.height);
	fullscreen.width = currentRes.width, fullscreen.height = currentRes.height;
	fullscreen.position = CP_Vector_Set(fullscreenWindowed.position.x, fullscreenWindowed.position.y + fullscreen.height);
	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;


	// Volume up and down buttons
	volumeDown.text = "-";
	volumeDown.width = back.width, volumeDown.height = back.width;
	volumeUp.text = "+";
	volumeUp.width = back.width, volumeUp.height = back.width;

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
	if (IsAreaClicked(back.position.x, back.position.y, back.width, back.height, mouse.x, mouse.y)) {
		CP_Settings_Tint(DARKGRAY);
	}

	// Return to main menu when click on back button
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.width, back.height, mouse.x, mouse.y)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	// Draw back button
	CP_Settings_Fill(RED);
	CP_Graphics_DrawRect(back.position.x, back.position.y, back.width, back.height);

	// Draw Resolution dropdown-list
	if (IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height, mouse.x, mouse.y)) {
		CP_Settings_Tint(DARKGRAY);
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height);
	}
	if (CP_Input_MouseClicked() && IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height, mouse.x, mouse.y)) {
		CP_Settings_NoTint();
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height);
		for (int i = 0; i < 3; i++) {
			CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].width, resolution[i].height);
		}
		CP_Settings_Fill(BLACK);
		CP_Settings_StrokeWeight(3.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(halfscreenWRes, window.x - PADDING, back.height + 2 * PADDING + currentRes.height);
		CP_Font_DrawText(fullscreenWRes, window.x - PADDING, back.height + 2 * PADDING + 2 * currentRes.height);
		CP_Font_DrawText(fullscreenRes, window.x - PADDING, back.height + 2 * PADDING + 3 * currentRes.height);
		ddlClicked = 1;
	}
	if (ddlClicked) {
		CP_Settings_NoTint();
		CP_Settings_Fill(DARKGRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height);
		for (int i = 0; i < 3; i++)
		{
			if (IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].width, resolution[i].height, mouse.x, mouse.y)) {
				CP_Settings_Tint(DARKGRAY);
				CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].width, resolution[i].height);
			}
			else {
				CP_Settings_NoTint();
				CP_Graphics_DrawRect(resolution[i].position.x, resolution[i].position.y, resolution[i].width, resolution[i].height);
			}
		}
		CP_Settings_Fill(BLACK);
		CP_Settings_StrokeWeight(3.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(halfscreenWRes, window.x - PADDING, back.height + 2 * PADDING + currentRes.height);
		CP_Font_DrawText(fullscreenWRes, window.x - PADDING, back.height + 2 * PADDING + 2 * currentRes.height);
		CP_Font_DrawText(fullscreenRes, window.x - PADDING, back.height + 2 * PADDING + 3 * currentRes.height);
	}
	if (CP_Input_MouseClicked() && !IsAreaClicked(currentRes.position.x, currentRes.position.y, currentRes.width, currentRes.height, mouse.x, mouse.y)) {
		ddlClicked = 0;
	}
	for (int i = 0; i < 3; i++) {
		if (CP_Input_MouseClicked() && IsAreaClicked(resolution[i].position.x, resolution[i].position.y, resolution[i].width, resolution[i].height, mouse.x, mouse.y)) {
			// replace current resolution
		}
	}
	CP_Settings_Fill(BLACK);
	CP_Settings_StrokeWeight(3.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(back.text, back.position.x, back.position.y);

	// Draw headers: Resolution, Volume, Tutorial
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText("Resolution", PADDING, back.height + 2 * PADDING);
	CP_Font_DrawText("Volume", PADDING, back.height + 3 * PADDING + textSize);
	CP_Font_DrawText("Tutorial", PADDING, back.height + 4 * PADDING + 2 * textSize);

	// Draw current resolution and resolution selection
	char currentRes[25] = { 0 };
	isWindowed ? sprintf_s(currentRes, _countof(currentRes), "%d x %d (windowed)", CP_System_GetWindowWidth(), CP_System_GetWindowHeight()) :
		sprintf_s(currentRes, _countof(currentRes), "%d x %d (fullscreen)", CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText(currentRes, window.x - PADDING, back.height + 2 * PADDING);

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
