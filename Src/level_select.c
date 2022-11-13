#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for Mouse Click Logic
#include "defines.h"		// Needed for define Values
#include "mainmenu.h"		// Needed to Return to Main Menu
#include "level_logic.h"	// Needed to use Global Extern Variable 'level'
#include "basegame.h"		// Needed to transit into Game Levels
#include "options_draw.h"

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

	float height = 0.f;
	char level_char[2] = {'\0'};

	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_NoTint();

	// Header Text
	CP_Settings_TextSize(windowwidth * .15f);
	drawAlignedText(WHITE, CENTER, "Select Level", windowwidth / 2, windowheight / 5);

	/* For-Loop to Draw Rectangles/Buttons for multiple Levels */
	for (int i = 0; i < 5; i++, height += 1.5f) {
		if ((i + 1) <= global_level) {
			btnColor = WHITE;
		}
		else {
			btnColor = GRAY;
		}
		drawTintedButton(btnColor, buttons.center_x, buttons.center_y + buttons.height * height, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);

		level_char[0] = (i + 1) + '0'; // To be set as Text for Level Number
		CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f); // Set Text Size scaling to Window Height
		drawAlignedText(BLACK, CENTER, level_char, buttons.center_x, buttons.center_y + buttons.height * height);
	}

	drawButton(back);

	// Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mousePos.x, mousePos.y)) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
		height = 0.f;
		/* For-Loop for Rectangle/Button On-Click Function Implementation */
		for (int j = 0; j < 5; j++, height += 1.25f) {
			if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * height, buttons.width, buttons.height, mousePos.x, mousePos.y)) {
				CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
				if (global_level > j) {
					set_temp_level(j+1); // Send over which level to load
					CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Load Level
				}
			}
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
