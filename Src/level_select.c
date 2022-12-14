/*
All content ? 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
		  Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
		  Guo Yiming (yiming.guo@digipen.edu)
			- Tinted Buttons
File	: level_select.c
Purpose	: State to select level
*/

#include "cprocessing.h"			// Needed for C Processing Functions
#include "utils.h"					// Needed for Mouse Click Logic
#include "defines.h"				// Needed for define Values
#include "mainmenu.h"				// Needed to Return to Main Menu
#include "level_logic.h"			// Needed to use Global Extern Variable 'level'
#include "basegame.h"				// Needed to transit into Game Levels
#include "easydraw.h"				// Needed for Customised Buttons & Texts
#include "spritesheet.h"			// Needed for background art
#include <stdio.h>					// Needed for sprintf()

extern Config config;				// To use resolution settings scaling
rect buttons;						// Button size
CP_Sound click;						// Click SFX
Button back;						// Customised Back Button Image/Icon

void Level_Select_Init(void)
{
	/* INITIALISE */
	load_background();																											// Load Background Image
	float imgSize = (float)config.settings.resolutionHeight/20.0f;																// Initialise Back Button image size
	setButton(&back, "./Assets/UI/Back.png", imgSize / 2.0f + PADDING, imgSize / 2.0f + PADDING, imgSize, imgSize, YES);		// Customised Back Button
}

void Level_Select_Update(void)
{
	/* INITIALISE VARIABLES */
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());												// Get Mouse Inputs into a Vector
	CP_Color btnColor = GRAY;																									// Set Button Color to GRAY

	float height = 0.f, column = 0.75f;
	char level_char[3] = {'\0'};
	int row = 0, col = 0, row_limit = 5;

	draw_background();																											// Draw Background Image
	CP_Settings_NoTint();																										// Clear any existing Tints

	// Header Text
	CP_Settings_TextSize((float)config.settings.resolutionWidth * .1f);
	drawAlignedText(BLACK, CENTER, "Select Level",																				// Draw 'Select Level' Text Aligned to the middle
		(float)config.settings.resolutionWidth / 2, (float)config.settings.resolutionHeight / 5);

	/* For-Loop to Draw Rectangles/Buttons for multiple Levels */
	for (col = 0; col < 2; col++, column += .5f) {
		for (; row < row_limit; row++, height += 1.5f) {
			if ((row + 1) <= global_level) {
				btnColor = PLYRRED;																								// If Level Unlocked, Button is RED
			}
			else {
				btnColor = GRAY;																								// If Level Locked, Button is GRAY
			}
			drawTintedButton(btnColor, buttons.center_x * column, buttons.center_y + buttons.height * height,					// Draw Customised Buttons		
				buttons.width, buttons.height, mousePos.x, mousePos.y, NO);

			sprintf_s(level_char, 3, "%d", (row + 1));																				// To be set as Text for Level Number
			CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.019f);															// Set Text Size scaling to Window Height
			drawAlignedText(WHITE, CENTER, level_char,																			// Draw Level Number Aligned to the middle of the button
				buttons.center_x * column, buttons.center_y + buttons.height * height);
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
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);														// Play Click SFX
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);										// Transit to Main Menu when Back Button is clicked
		}
		height = 0.f;
		/* For-Loop for Rectangle/Button On-Click Function Implementation */
		for (col = 0; col < 2; col++, column += .5f) {
			for (; row < row_limit; row++, height += 1.5f) {
				if (IsAreaClicked(buttons.center_x * column, buttons.center_y + buttons.height * height, buttons.width, buttons.height, mousePos.x, mousePos.y)) {
					CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);												// Play Click SFX
					if (global_level > row) {
						set_temp_level(row + 1);																				// Send over which level to load
						CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit);											// Load Level
					}
				}
			}
			row_limit += 5;
			height = 0.f;
		}
		
	}

	// Returns to Main Menu if Player hits 'ESC'
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);															// Play Click SFX
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);											// Transit to Main Menu when ESC is Pressed
	}

}

void Level_Select_Exit(void)
{
	//CP_Sound_Free(&click);
	CP_Image_Free(&back.img);																									// Free Back Button Image
	free_background();																											// Free Background Image
}
