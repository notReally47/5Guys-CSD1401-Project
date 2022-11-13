#define _CRT_SECURE_NO_WARNINGS		// Needed to use sprintf() instead of sprintf_s()
#define _CRT_SECURE_NO_DEPRECATE	// Needed to use sprintf() instead of sprintf_s()
#include "cprocessing.h"			// Needed for C Processing Functions
#include "utils.h"					// Needed for Mouse Click Logic
#include "defines.h"				// Needed for define Values
#include "mainmenu.h"				// Needed to Return to Main Menu
#include "level_logic.h"			// Needed to use Global Extern Variable 'level'
#include "basegame.h"				// Needed to transit into Game Levels
#include "options_draw.h"
#include <stdio.h>

extern Config config;
float windowwidth, windowheight;
extern rect buttons;
CP_Sound click;
Button back;

void Level_Select_Init()
{
	/*INITIALISE*/
	float imgSize = (float)(CP_System_GetWindowHeight() / 20.0f);
	setButton(&back, "./Assets/UI/Back.png", imgSize / 2.0f + PADDING, imgSize / 2.0f + PADDING, imgSize, imgSize, YES);
}

void Level_Select_Update()
{
	/*INITIALISE VARIABLES*/
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	CP_Color btnColor = GRAY;

	float height = 0.f, column = 0.75f;
	char level_char[3] = {'\0'};
	int row = 0, col = 0, row_limit = 5;

	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_NoTint();

	// Header Text
	CP_Settings_TextSize(windowwidth * .15f);
	drawAlignedText(WHITE, CENTER, "Select Level", windowwidth / 2, windowheight / 5);

	/* For-Loop to Draw Rectangles/Buttons for multiple Levels */
	for (col = 0; col < 2; col++, column += .5f) {
		for (; row < row_limit; row++, height += 1.5f) {
			if ((row + 1) <= global_level) {
				btnColor = WHITE;
			}
			else {
				btnColor = GRAY;
			}
			drawTintedButton(btnColor, buttons.center_x * column, buttons.center_y + buttons.height * height, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);

			sprintf(level_char, "%d", (row + 1));	// To be set as Text for Level Number
			CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f); // Set Text Size scaling to Window Height
			drawAlignedText(BLACK, CENTER, level_char, buttons.center_x * column, buttons.center_y + buttons.height * height);
		}
		row_limit += 5;
		height = 0.f;
	}
	
	row = 0;
	row_limit = 5;
	height = 0.f;
	column = 0.75f;

	drawButton(back);

	// Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mousePos.x, mousePos.y)) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
		height = 0.f;
		/* For-Loop for Rectangle/Button On-Click Function Implementation */
		for (col = 0; col < 2; col++, column += .5f) {
			for (; row < row_limit; row++, height += 1.5f) {
				if (IsAreaClicked(buttons.center_x * column, buttons.center_y + buttons.height * height, buttons.width, buttons.height, mousePos.x, mousePos.y)) {
					CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
					if (global_level > row) {
						set_temp_level(row + 1); // Send over which level to load
						CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Load Level
					}
				}
			}
			row_limit += 5;
			height = 0.f;
		}
		
	}

	// Returns to Main Menu if Player hits 'ESC'
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

}

void Level_Select_Exit()
{
	CP_Image_Free(&back.img);
}
