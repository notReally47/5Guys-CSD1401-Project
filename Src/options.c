/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Guo Yiming (yiming.guo@digipen.edu)
			- Design
			- Drawing
			- Logic for resolution/volume changes
			- Write resolution/volume changes to config.dat
		  Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
File	: options.c
Purpose	: Setting page for player to change their settings
*/

#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "structs.h"
#include "easydraw.h"
#include "settings.h"
#include "options.h"
#include "spritesheet.h" // for background art
#include <stdio.h> // sprintf_s()
#include <stdlib.h> //_countof()
#include <string.h> // strlen()
#include "utils.h" // IsTaskbarWndVisible & getTaskBarHeight

extern Config config;
Config newConfig;

Button back, volumeDown, volumeUp, apply, discard, controls[9], up, down, left, right, pause, undo, reset, escape, camera, btns[14];
DropDownList currentRes, resolution[3], halfscreenWindowed, fullscreenWindowed, fullscreen, * resSelected, * initialRes;

GIF gameplay;
static float timeElapsed;
static const float displayDuration = .1f;

CP_Vector window;
Flag ddlClicked, volChanged, resChanged, configChanged, resUnmatch;
float textSize, imgSize;
int displayVol;

CP_Sound click;

void Options_Init() {
	/*Initialize config*/
	newConfig = config;
	displayVol = newConfig.settings.audio;

	/*Initialize global variables*/
	imgSize = (float)(CP_System_GetWindowHeight() / 20.0f), textSize = imgSize;
	window = CP_Vector_Set((float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());

	/*Logic Flags*/
	ddlClicked = NO;
	volChanged = NO;
	resChanged = NO;
	configChanged = NO;

	/*Settings*/
	CP_Settings_TextSize(textSize);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(BLACK);

	/*Create Buttons*/
	setButton(&back, "./Assets/UI/Back.png",	imgSize / 2.0f + PADDING, imgSize / 2.0f + PADDING,imgSize, imgSize, YES);
	setButton(&volumeDown, "./Assets/UI/VolumeDown.png", window.x - 3 * PADDING - 2.5 * textSize, imgSize + 3 * PADDING + 1.5 * textSize, imgSize, imgSize, YES);
	setButton(&volumeUp, "./Assets/UI/VolumeUp.png",	window.x - PADDING - textSize / 2, imgSize + 3 * PADDING + 1.5 * textSize, imgSize, imgSize, YES);
	setButton(&apply, "./Assets/UI/Apply.png", window.x - PADDING - 4 * imgSize, window.y - PADDING - 1.5 * imgSize, 2 * imgSize, 2 * imgSize, YES);
	setButton(&discard, "./Assets/UI/Discard.png", window.x - PADDING - imgSize, window.y - PADDING - 1.5 * imgSize, 2 * imgSize, 2 * imgSize, YES);

	setButton(&up, "./Assets/UI/W.png", PADDING + 0.75 * imgSize, PADDING + imgSize + 5 * textSize, 1.5 * imgSize, 1.5 *imgSize, NO);
	setButton(&left, "./Assets/UI/A.png", up.position.x + 10 * imgSize, up.position.y , 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&down, "./Assets/UI/S.png", up.position.x, up.position.y + 2 * imgSize, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&right, "./Assets/UI/D.png", left.position.x, down.position.y, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&pause, "./Assets/UI/P.png", down.position.x, down.position.y + 2 * imgSize, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&escape, "./Assets/UI/ESC.png", pause.position.x + 2 * imgSize, pause.position.y, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&undo, "./Assets/UI/U.png", pause.position.x, pause.position.y + 2 * imgSize, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&reset, "./Assets/UI/R.png", undo.position.x, undo.position.y + 2 * imgSize, 1.5 * imgSize, 1.5 * imgSize, NO);
	setButton(&camera, "./Assets/UI/C.png", reset.position.x, reset.position.y + 2 * imgSize, 1.5 * imgSize, 1.5 * imgSize, NO);

	controls[0] = up, controls[1] = left, controls[2] = down, controls[3] = right, controls[4] = pause, controls[5] = escape, controls[6] = undo, controls[7] = reset, controls[8] = camera;

	btns[0] = back, btns[1] = volumeDown, btns[2] = volumeUp, btns[3] = apply, btns[4] = discard, btns[5] = up, btns[6] = left, btns[7] = down, btns[8] = right, btns[9] = pause, btns[10] = escape, btns[11] = undo, btns[12] = reset, btns[13] = camera;

	/*Create Dropdown List*/
	setDropDownList(&currentRes, newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight, newConfig.settings.windowed, window.x - (MAX_LENGTH * textSize / 2 + PADDING) / 2 - PADDING, 2 * PADDING + back.btnHeight + textSize / 2, MAX_LENGTH * textSize / 2 + PADDING, textSize);
	setDropDownList(&halfscreenWindowed, (unsigned int)(CP_System_GetDisplayWidth() / 2), (unsigned int)(CP_System_GetDisplayHeight() / 2), YES, currentRes.button.position.x, currentRes.button.position.y + currentRes.button.btnHeight, currentRes.button.btnWidth, currentRes.button.btnHeight);
	IsTaskbarWndVisible() ? setDropDownList(&fullscreenWindowed, (unsigned int)(CP_System_GetDisplayWidth()), (unsigned int)(CP_System_GetDisplayHeight() - getTaskBarHeight() - newConfig.settings.titleBarHeight), YES, halfscreenWindowed.button.position.x, halfscreenWindowed.button.position.y + currentRes.button.btnHeight, currentRes.button.btnWidth, currentRes.button.btnHeight) :
	setDropDownList(&fullscreenWindowed, (unsigned int)(CP_System_GetDisplayWidth()), (unsigned int)(CP_System_GetDisplayHeight()), YES, halfscreenWindowed.button.position.x, halfscreenWindowed.button.position.y + currentRes.button.btnHeight, currentRes.button.btnWidth, currentRes.button.btnHeight);
	setDropDownList(&fullscreen, (unsigned int)(CP_System_GetDisplayWidth()), (unsigned int)(CP_System_GetDisplayHeight()), NO, fullscreenWindowed.button.position.x, fullscreenWindowed.button.position.y + currentRes.button.btnHeight, currentRes.button.btnWidth, currentRes.button.btnHeight);
	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;

	/*Set selected resolution*/
	for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++) {
		resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? 1 : 0;
	}
	/*If none matches config, set default to half screen windowed*/
	if (!((resolution[0].selected) || (resolution[1].selected) || (resolution[2].selected))) {
		resolution[0].selected = YES;
		resUnmatch = YES;
	}
	for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++) {
		if (resolution[i].selected) {
			resSelected = &resolution[i];
		}
	}
	initialRes = resSelected;

	/*GIF*/
	timeElapsed = 0.0f;
	setGIF(&gameplay, "./Assets/Spritesheet/gameplay_white.png", 5, 9, window.x - PADDING - imgSize * 10, up.position.y - up.btnHeight / 2, imgSize * 10);
	
	/*Background art*/
	load_background();
	/*-----------------------------------*/
}

void Options_Update() {
	CP_Vector mouse = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()); // mouse position
	CP_Color currentResColor = ddlClicked ? BGDBLUE : BGLBLUE;
	timeElapsed += CP_System_GetDt();
	CP_Settings_NoTint();

	/*INPUT*/
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

	/*Select resolution*/
	if (ddlClicked) {
		if(CP_Input_MouseClicked())
		{
			for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++) {
				if (IsAreaClicked(resolution[i].button.position.x, resolution[i].button.position.y, resolution[i].button.btnWidth, resolution[i].button.btnHeight, mouse.x, mouse.y)) {
					CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
					resolution[i].selected = YES;
					ddlClicked = NO;
					resChanged = (resolution[i].actWidth == currentRes.actWidth && resolution[i].actHeight == currentRes.actHeight && resolution[i].windowed == currentRes.windowed) ? NO : YES;
					configChanged = resChanged || volChanged ? YES : NO;
				}
				else resolution[i].selected = NO;
			}
			/*Retract dropdown list*/
			if (!IsAreaClicked(currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y)) {
				CP_Sound_PlayAdvanced(click, 1 / 100.f, 2, FALSE, CP_SOUND_GROUP_SFX);
				ddlClicked = NO;
			}
		}
	}

	else {
		/*Adjusting Volume*/
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) {
			if (!(CP_System_GetFrameCount() % 5))
			{
				/*Vol down*/
				if (IsAreaClicked(volumeDown.position.x, volumeDown.position.y, volumeDown.btnWidth, volumeDown.btnHeight, mouse.x, mouse.y)) {
					if (displayVol > 0) {
						CP_Sound_PlayAdvanced(click, --displayVol / 100.f, 2, NO, CP_SOUND_GROUP_2);
					}
					volChanged = displayVol == newConfig.settings.audio ? NO : YES;
					configChanged = resChanged || volChanged ? YES : NO;
				}
				/*Vol up*/
				else if (IsAreaClicked(volumeUp.position.x, volumeUp.position.y, volumeUp.btnWidth, volumeUp.btnHeight, mouse.x, mouse.y)) {
					if (displayVol < 100) {
						CP_Sound_PlayAdvanced(click, ++displayVol / 100.f, 2, NO, CP_SOUND_GROUP_2);
					}
					volChanged = displayVol == newConfig.settings.audio ? NO : YES;
					configChanged = resChanged || volChanged ? YES : NO;
				}
			}
		}

		/*Button clicks*/
		if (CP_Input_MouseClicked()) {

			/*Return to main menu when click on back button*/
			if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y)) {
				CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}

			/*Display dropdown list*/
			if (IsAreaClicked(currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y)) {
				CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
				ddlClicked = YES;
			}
		}

		/*Apply and Discard Changes*/
		if (configChanged) {
			if (CP_Input_MouseClicked()) {
				/*Apply*/
				if (IsAreaClicked(apply.position.x, apply.position.y, apply.btnWidth, apply.btnHeight, mouse.x, mouse.y)) {
					CP_Sound_PlayAdvanced(click, newConfig.settings.audio / 100.f, 2, FALSE, CP_SOUND_GROUP_SFX);
					(resSelected->actHeight == fullscreenWindowed.actHeight && resSelected->actWidth == fullscreenWindowed.actWidth) ? CP_System_SetWindowPosition(0, newConfig.settings.titleBarHeight) :
						CP_System_SetWindowPosition(window.x / 4, window.y / 4);
					newConfig.settings.resolutionWidth = resSelected->actWidth;
					newConfig.settings.resolutionHeight = resSelected->actHeight;
					newConfig.settings.windowed = resSelected->windowed;
					newConfig.settings.audio = displayVol;
					newConfig.settings.windowed ? CP_System_SetWindowSize(newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight) : CP_System_Fullscreen();

					/*Overwrite settings*/
					config = newConfig;
					writeConfig(newConfig);

					/*Set volume*/
					CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, displayVol / 100.f);
					CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, displayVol / 100.f);

					configChanged = NO;
					resUnmatch = NO;
					CP_Engine_SetNextGameStateForced(Options_Init, Options_Update, Options_Exit); // Reload game state to rescale
				}
				/*Discard*/
				else if (IsAreaClicked(discard.position.x, discard.position.y, discard.btnWidth, discard.btnHeight, mouse.x, mouse.y)) {
					CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
					displayVol = newConfig.settings.audio;
					for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++) {
						resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? YES : NO;
					}
					configChanged = NO;
					resChanged = NO;
				}
			}
		}
	}

	/*LOGIC*/

	/*Set selected resolution*/
	for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++)
	{
		if (resolution[i].selected) {
			resSelected = &resolution[i];
		}
	}

	/*RENDER*/

	/*Clears and draws background art*/
	draw_background();
	/*char buff[25] = { 0 };
	sprintf_s(buff, _countof(buff), "Height of Title Bar: %d", newConfig.settings.titleBarHeight);
	drawAlignedText(RED, CENTER, buff, window.x / 2, window.y - window.y / 4);*/

	/*Resolution text*/
	const char* resList[3];
	char halfscreenWRes[25] = { 0 };
	sprintf_s(halfscreenWRes, _countof(halfscreenWRes), "%u x %u (windowed)", halfscreenWindowed.actWidth, halfscreenWindowed.actHeight);
	char fullscreenWRes[25] = { 0 };
	sprintf_s(fullscreenWRes, _countof(fullscreenWRes), "%u x %u (windowed)", fullscreenWindowed.actWidth, fullscreenWindowed.actHeight);
	char fullscreenRes[25] = { 0 };
	sprintf_s(fullscreenRes, _countof(fullscreenRes), "%u x %u (fullscreen)", fullscreen.actWidth, fullscreen.actHeight);
	resList[0] = halfscreenWRes, resList[1] = fullscreenWRes, resList[2] = fullscreenRes;

	/*Draw apply and discard*/
	if (configChanged) {
		drawButton(apply);
		drawButton(discard);
	}

	/*Draw back button*/
	drawButton(back);

	/*Draw headers: Resolution, Volume, Tutorial*/
	const char* headers[3];
	headers[0] = "Resolution", headers[1] = "Volume", headers[2] = "Controls";
	drawHeader(headers, 3);

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

	char displayRes[25] = { 0 };

	sprintf_s(displayRes, _countof(displayRes), "%d x %d (%s)",
		resUnmatch && !resChanged ? config.settings.resolutionWidth : resSelected->actWidth,
		resUnmatch && !resChanged ? config.settings.resolutionHeight : resSelected->actHeight,
		resUnmatch && !resChanged ? config.settings.windowed ? "windowed" : "fullscreen" : resSelected->windowed ? "windowed" : "fullscreen");

	/*resUnmatch && !resChanged ?
		sprintf_s(displayRes, _countof(displayRes), "%d x %d (%s)",
		config.settings.resolutionWidth, config.settings.resolutionHeight, config.settings.windowed ? "windowed" : "fullscreen") :
		sprintf_s(displayRes, _countof(displayRes), "%d x %d (%s)",
			resSelected->actWidth, resSelected->actHeight, resSelected->windowed ? "windowed" : "fullscreen");*/

	/*Draw current volume and the volume up and down buttons*/
	char currentVol[16] = { 0 };
	sprintf_s(currentVol, _countof(currentVol), "%d", displayVol);
	drawButton(volumeUp);
	drawButton(volumeDown);
	drawAlignedText(BLACK, CENTER, currentVol, volumeDown.position.x + ((volumeUp.position.x - volumeDown.position.x) / 2), back.btnHeight + 3 * PADDING + 1.5 * textSize);

	/*Draw Gameplay GIF & Controls*/
	drawGIF(&gameplay, &timeElapsed, displayDuration, YES, NO);
	for (int i = 0, x = imgSize, y = imgSize / 2; i < sizeof(controls) / sizeof(Button); i++) {
		drawButton(controls[i]);
		drawAlignedText(BLACK, LEFT, controlDescription[i], controls[i].position.x + x, controls[i].position.y - y);
	}

	/*Draw Resolution dropdown-list*/
	drawTintedButton(currentResColor, currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y, YES);
	drawAlignedText(BLACK, RIGHT, displayRes, window.x - PADDING, back.btnHeight + 2 * PADDING);

	if (ddlClicked) {
		float textX = window.x - 2 * PADDING, textY = back.btnHeight + 2 * PADDING + currentRes.button.btnHeight;
		for (int i = 0; i < sizeof(resolution) / sizeof(DropDownList); i++, textY += currentRes.button.btnHeight) {
			drawTintedButton(BGLBLUE, resolution[i].button.position.x, resolution[i].button.position.y, resolution[i].button.btnWidth, resolution[i].button.btnHeight, mouse.x, mouse.y, YES);
			drawAlignedText(BLACK, RIGHT, resList[i], textX, textY);
		}
		//drawAlignedText(WHITE, RIGHT, displayRes, window.x - PADDING, back.btnHeight + 2 * PADDING);
	}
}

void Options_Exit() {
	freeButtonImg(btns, 14);
	CP_Image_Free(&gameplay.spritesheet);
	free_background();
}
