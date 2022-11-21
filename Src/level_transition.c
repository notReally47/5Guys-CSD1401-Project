/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
		  Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
			- Level Complete SFX
		  Guo Yiming (yiming.guo@digipen.edu)
			- Tinted Buttons
File	: level_transition.c
Purpose	: State for level complete and transiting to next level & choosing of Buff/Debuff
*/

#include "cprocessing.h"			// Needed for C Processing Functions
#include "utils.h"					// Needed for Mouse Click Logic
#include "defines.h"				// Needed for define Values
#include "mainmenu.h"				// Needed to Return to Main Menu
#include "basegame.h"				// Needed to Transit to Next Level
#include "mechanics.h"				// Needed for card_selection()
#include "level_logic.h"			// Needed for level global variable
#include "spritesheet.h"			// Needed for Unique Mechanics
#include "easydraw.h"				// Needed for Customised Buttons & Texts
#include "settings.h"				// Needed to read/write to config.dat

extern Config config;				// To use resolution settings scaling
rect buttons;						// Button size
Flag applied;						// For Card Check
CP_Sound nextLvl = NULL, click;		// SFX for Next Level & CLick

void Level_Transition_Init(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);										// Align rectangle to the center position (else it defaults to top left corner)

	/* Define Button Size and Position */
	buttons.center_x = (float)config.settings.resolutionWidth * 0.5f;
	buttons.center_y = (float)config.settings.resolutionHeight * 0.5f;
	buttons.width = (float)config.settings.resolutionWidth * 0.1f;
	buttons.height = (float)config.settings.resolutionHeight * 1.f / 12.f;

	applied = NO;

	/* Load & Play Next Level SFX */
	nextLvl = CP_Sound_Load("./Assets/Sound/SFX/Level.wav");						// Load Next Level SFX
	CP_Sound_PlayAdvanced(nextLvl, 1, 1, FALSE, CP_SOUND_GROUP_SFX);				// Play Next Level SFX

	/* Load Background Image */
	load_background();														

	/* Load Card Settings */
	card_settings();
}

void Level_Transition_Update(void)
{
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());				// Get Mouse Inputs into a Vector
	CP_Color lvlComplete = !(CP_System_GetFrameCount() % 5) ? PLYRRED : PLYRGRN;				// Change color every 5 frames (Flashing Text)
	draw_background();																			// Clears & Draws Background

	for (int i = 2; i < 10; i++) {
		if (global_level == i) {
			if (!(UM.selectedflag & 2 << (i - 2))) {
				card_selection(i%2, &applied);
				config.UM = UM;
				writeConfig(config);
			}
			else
				applied = YES;
		}
	}
	if (global_level == 1 || global_level == 10)
		applied = YES;
	
	/* Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles */
	if (CP_Input_MouseClicked() && applied) { 
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);												// Play Click SFX
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit);												// Go to the Next Level
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);												// Play Click SFX
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);								// Return to Main Menu
		}
	}
	if (applied) {
		drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);							// Draw Next Button as RED once Card Confirmed
		drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);	// Draw Main Menu as RED once Card Confirmed
	}
	else {
		CP_Settings_Fill(GRAY);																								// Set Button Color to GRAY
		CP_Settings_StrokeWeight(0.0f);																						// Remove any strokes
		CP_Graphics_DrawRect(buttons.center_x, buttons.center_y, buttons.width, buttons.height);							// Draw Next Button GRAY
		CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height);	// Draw Main Manu Button GRAY
	}

	// Draw Texts 'Next' & 'Main Menu' on Rectangles
	CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f);																// Set Text Size scaling to Window Height
	drawAlignedText(WHITE, CENTER, "Next", buttons.center_x, buttons.center_y);												// Draw 1st Text as 'Next
	drawAlignedText(WHITE, CENTER, "Main Menu", buttons.center_x, buttons.center_y + buttons.height * 1.5f);				// Draw 2nd Text as 'Main Menu'

	// Draw Text 'Level Complete!"
	CP_Settings_TextSize((float)CP_System_GetWindowWidth() * .1f);															// Set 'Level Complete!' Text Font Size
	drawAlignedText(lvlComplete, CENTER, "Level Complete!",																	// Draw 'Level Complete!' Text
		(float)CP_System_GetWindowWidth() / 2, (float)config.settings.resolutionHeight / 5);	
}

void Level_Transition_Exit(void)
{
	free_buttons();
	free_background();
	CP_Sound_Free(&nextLvl);
}
