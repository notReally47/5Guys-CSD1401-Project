#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "structs.h"
#include "options_draw.h"
#include "settings.h"
#include <stdio.h> // sprintf_s()
#include <stdlib.h> //_countof()
#include <string.h> // strlen() & strcpy_s()
#include "options.h"

Button back, volumeDown, volumeUp, changes[2], apply, discard;
DropDownList currentRes, resolution[3], halfscreenWindowed, fullscreenWindowed, fullscreen, * resSelected;
float textSize, volume, numCols, numRows, imgSize;

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
CP_Sound gameSFX;
//CP_Sound gameMusic;
CP_Font gameFont;
int ddlClicked, configChanged, displayVol;

extern Config config;
Config newConfig;

void Options_Init() {
	/*Initialize config*/
	//config = readFile();
	imgSize = CP_System_GetWindowHeight() / 20;
	newConfig = config;
	CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
	window = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	textSize = (float)window.y * 0.05f;
	gameplay = CP_Image_Load("./Assets/Gameplay.png");

	//gameMusic = CP_Sound_Load("./Assets/Sound/music.mp3");
	gameSFX = CP_Sound_Load("./Assets/Sound/sfx.wav");

	/*gameFont = CP_Font_Load("./Assets/Fonts/VT323-Regular.ttf");
	CP_Font_Set(gameFont);*/
	CP_Settings_TextSize(textSize);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(BLACK);

	// Initialize constant struct fields
	back.img = CP_Image_Load("./Assets/assetsTmp/back.png"); // to be replaced with an image

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

	volumeDown.img = CP_Image_Load("./Assets/assetsTmp/volDown.png");
	volumeUp.img = CP_Image_Load("./Assets/assetsTmp/volUp.png");

	apply.img = CP_Image_Load("./Assets/assetsTmp/apply.png");
	discard.img = CP_Image_Load("./Assets/assetsTmp/discard.png");

	// Dynamic struct fields (change due to resolution change)

	/*-------------BUTTONS-------------*/
	back.btnWidth = imgSize, back.btnHeight = imgSize;
	back.position = CP_Vector_Set(back.btnWidth / 2.0f + PADDING, back.btnHeight / 2.0f + PADDING);

	volumeDown.btnWidth = imgSize, volumeDown.btnHeight = imgSize;
	volumeDown.position = CP_Vector_Set(window.x - 3 * PADDING - 2.5 * textSize, back.btnHeight + 3 * PADDING + 1.5 * textSize);
	volumeUp.btnWidth = imgSize, volumeUp.btnHeight = imgSize;
	volumeUp.position = CP_Vector_Set(window.x - PADDING - textSize / 2, back.btnHeight + 3 * PADDING + 1.5 * textSize);

	apply.btnWidth = imgSize, apply.btnHeight = imgSize;
	apply.position = CP_Vector_Set(window.x / 2 - 5 * PADDING - apply.btnWidth / 2, window.y - PADDING - apply.btnHeight / 2);

	discard.btnWidth = imgSize, discard.btnHeight = imgSize;
	discard.position = CP_Vector_Set(window.x / 2 + 5 * PADDING + discard.btnWidth / 2, window.y - PADDING - discard.btnHeight / 2);

	changes[0] = apply, changes[1] = discard;

	/*-----------------------------------*/

	/*-------------RESOLUTION-------------*/
	currentRes.button.btnWidth = MAX_LENGTH * textSize / 2 + PADDING, currentRes.button.btnHeight = textSize;
	currentRes.button.position = CP_Vector_Set(window.x - currentRes.button.btnWidth / 2 - PADDING, 2 * PADDING + back.btnHeight + textSize / 2);

	halfscreenWindowed.button.btnWidth = currentRes.button.btnWidth, halfscreenWindowed.button.btnHeight = currentRes.button.btnHeight;
	halfscreenWindowed.button.position = CP_Vector_Set(currentRes.button.position.x, currentRes.button.position.y + halfscreenWindowed.button.btnHeight);

	fullscreenWindowed.button.btnWidth = currentRes.button.btnWidth, fullscreenWindowed.button.btnHeight = currentRes.button.btnHeight;
	fullscreenWindowed.button.position = CP_Vector_Set(halfscreenWindowed.button.position.x, halfscreenWindowed.button.position.y + fullscreenWindowed.button.btnHeight);

	fullscreen.button.btnWidth = currentRes.button.btnWidth, fullscreen.button.btnHeight = currentRes.button.btnHeight;
	fullscreen.button.position = CP_Vector_Set(fullscreenWindowed.button.position.x, fullscreenWindowed.button.position.y + fullscreen.button.btnHeight);

	resolution[0] = halfscreenWindowed, resolution[1] = fullscreenWindowed, resolution[2] = fullscreen;
	/*-----------------------------------*/

	// Gif
	imageIndex = 0;
	timeElapsed = 0.0f;

	// Dropdown-list
	ddlClicked = NO;
	configChanged = NO;

	/*Set selected resolution*/
	for (int i = 0; i < 3; i++) {
		resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? 1 : 0;
	}

	displayVol = newConfig.settings.audio;

	//CP_Sound_PlayAdvanced(gameMusic, config.settings.audio / 100.f, 1, YES, CP_SOUND_GROUP_MUSIC);
}

void Options_Update() {
	CP_Vector mouse = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()); // mouse position
	CP_Color currentResColor = ddlClicked ? DARKGRAY : GRAY;
	CP_Settings_NoTint();
	CP_Graphics_ClearBackground(GRAY);

	/*INPUT*/
	/*Adjusting Volume*/
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) {
		if (!(CP_System_GetFrameCount() % 5))
		{
			/*Vol down*/
			if (IsAreaClicked(volumeDown.position.x, volumeDown.position.y, volumeDown.btnWidth, volumeDown.btnHeight, mouse.x, mouse.y)) {
				if (displayVol > 0) {
					CP_Sound_PlayAdvanced(gameSFX, --displayVol / 100.f, 1, NO, CP_SOUND_GROUP_SFX);
				}
				configChanged = displayVol == newConfig.settings.audio ? 0 : 1;
			}
			/*Vol up*/
			else if (IsAreaClicked(volumeUp.position.x, volumeUp.position.y, volumeUp.btnWidth, volumeUp.btnHeight, mouse.x, mouse.y)) {
				if (displayVol < 100) {
					CP_Sound_PlayAdvanced(gameSFX, ++displayVol / 100.f, 1, NO, CP_SOUND_GROUP_SFX);
				}
				configChanged = displayVol == newConfig.settings.audio ? 0 : 1;
			}
		}
	}

	if (ddlClicked) {
		if(CP_Input_MouseClicked())
		{
			for (int i = 0; i < 3; i++) {
				if (IsAreaClicked(resolution[i].button.position.x, resolution[i].button.position.y, resolution[i].button.btnWidth, resolution[i].button.btnHeight, mouse.x, mouse.y)) {
					resolution[i].selected = YES;
					ddlClicked = NO;
					configChanged = (resolution[i].actWidth == currentRes.actWidth && resolution[i].actHeight == currentRes.actHeight && resolution[i].windowed == currentRes.windowed) ? NO : YES;
				}
				else resolution[i].selected = NO;
			}
		}
	}

	if (CP_Input_MouseClicked()) {

		/*Retract dropdown list*/
		if (!IsAreaClicked(currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y)) {
			ddlClicked = NO;
		}

		/*Return to main menu when click on back button*/
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mouse.x, mouse.y)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}

		/*Display dropdown list*/
		if (IsAreaClicked(currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y)) {
			ddlClicked = YES;
		}

	}

	/*Apply and Discard Changes*/
	if (configChanged) {
		if (CP_Input_MouseClicked()) {
			for (int i = 0; i < 2; i++)
			{
				/*Apply*/
				if (IsAreaClicked(changes[APPLY].position.x, changes[APPLY].position.y, changes[APPLY].btnWidth, changes[APPLY].btnHeight, mouse.x, mouse.y)) {
					newConfig.settings.resolutionWidth = resSelected->actWidth;
					newConfig.settings.resolutionHeight = resSelected->actHeight;
					newConfig.settings.windowed = resSelected->windowed;
					newConfig.settings.audio = displayVol;
					newConfig.settings.windowed ? CP_System_SetWindowSize(newConfig.settings.resolutionWidth, newConfig.settings.resolutionHeight) : CP_System_Fullscreen();
					//writeConfig(newConfig);

					//CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, displayVol / 100.f);
					CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, displayVol / 100.f);

					configChanged = NO;
					//CP_Engine_SetNextGameStateForced(Options_Init, Options_Update, Options_Exit); // Reload game state to rescale
				}
				/*Discard*/
				else if (IsAreaClicked(changes[DISCARD].position.x, changes[DISCARD].position.y, changes[DISCARD].btnWidth, changes[DISCARD].btnHeight, mouse.x, mouse.y)) {
					displayVol = newConfig.settings.audio;
					for (int i = 0; i < 3; i++) {
						resolution[i].selected = (resolution[i].actWidth == config.settings.resolutionWidth && resolution[i].actHeight == config.settings.resolutionHeight && resolution[i].windowed == config.settings.windowed) ? 1 : 0;
					}
					configChanged = NO;
				}
			}
		}
	}

	/*LOGIC*/

	/*Set selected resolution*/
	for (int i = 0; i < 3; i++)
	{
		if (resolution[i].selected) {
			resSelected = &resolution[i];
		}
	}

	/*Draw gameplay gif logic*/
	timeElapsed += CP_System_GetDt();
	if (timeElapsed >= DISPLAY_DURATION) {
		imageIndex = (imageIndex + 1) % TOTAL_FRAMES;
		timeElapsed = 0.0f;
	}
	gifDimension = window.y * .6f;

	/*RENDER*/

	/*Resolution text*/
	const char* a[3];
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

	/*Draw apply and discard*/
	if (configChanged) {
		for (int i = 0; i < 2; i++)
		{
			drawBtn(changes[i]);
		}
	}

	/*Draw back button*/
	drawBtn(back);

	/*Draw headers: Resolution, Volume, Tutorial*/
	drawAlignedText(BLACK, LEFT, "Resolution", PADDING, back.btnHeight + 2 * PADDING);
	drawAlignedText(BLACK, LEFT, "Volume", PADDING, back.btnHeight + 3 * PADDING + textSize);
	drawAlignedText(BLACK, LEFT, "Tutorial", PADDING, back.btnHeight + 4 * PADDING + 2 * textSize);

	char displayRes[25] = { 0 };
	resSelected->windowed ? sprintf_s(displayRes, _countof(displayRes), "%d x %d (windowed)", resSelected->actWidth, resSelected->actHeight) :
		sprintf_s(displayRes, _countof(displayRes), "%d x %d (fullscreen)", resSelected->actWidth, resSelected->actHeight);

	/*Draw current volume and the volume up and down buttons*/
	char currentVol[16] = { 0 };
	sprintf_s(currentVol, _countof(currentVol), "%d", displayVol);
	drawBtn(volumeUp);
	drawBtn(volumeDown);
	drawAlignedText(BLACK, CENTER, currentVol, volumeDown.position.x + ((volumeUp.position.x - volumeDown.position.x) / 2), back.btnHeight + 3 * PADDING + 1.5 * textSize);

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
	drawTintedButton(currentResColor, currentRes.button.position.x, currentRes.button.position.y, currentRes.button.btnWidth, currentRes.button.btnHeight, mouse.x, mouse.y, YES);
	drawAlignedText(BLACK, RIGHT, displayRes, window.x - 2 * PADDING, back.btnHeight + 2 * PADDING);

	if (ddlClicked) {
		float textX = window.x - 2 * PADDING, textY = back.btnHeight + 2 * PADDING + currentRes.button.btnHeight;
		for (int i = 0; i < 3; i++, textY += currentRes.button.btnHeight) {
			drawTintedButton(GRAY, resolution[i].button.position.x, resolution[i].button.position.y, resolution[i].button.btnWidth, resolution[i].button.btnHeight, mouse.x, mouse.y, YES);
			drawAlignedText(BLACK, RIGHT, resolution[i].name, textX, textY);
		}
		CP_Font_DrawText(displayRes, window.x - 2 * PADDING, back.btnHeight + 2 * PADDING);
	}
}

void Options_Exit() {
	CP_Image_Free(&gameplay);
	//CP_Sound_Free(&gameMusic);
	CP_Sound_Free(&gameSFX);

	/*Update config to newConfig*/
	config = newConfig;
}
