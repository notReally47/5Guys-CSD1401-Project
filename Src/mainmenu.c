/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
			- Main Menu BGM
File	: mainmenu.c
Purpose	: Main Menu Page
*/

#include "cprocessing.h"
#include "utils.h"
#include "credits.h"
#include "defines.h"
#include "basegame.h"
#include "options.h"
#include "level_logic.h"
#include "level_select.h"
#include "spritesheet.h"
#include "easydraw.h"
#include "mechanics.h"

extern Config config;
CP_Sound click = NULL, gameMusic;

rect buttons;

void Main_Menu_Init()
{
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	CP_Settings_Stroke(BLACK); // black border around the rectangle
	// define buttons
	buttons.center_x = (float)config.settings.resolutionWidth * 0.5f;
	buttons.center_y = (float)config.settings.resolutionHeight * 0.4f;
	buttons.width = (float)config.settings.resolutionWidth * 0.1f;
	buttons.height = (float)config.settings.resolutionHeight * 1.f / 15.f;

	load_background();

	// SFX
	click = CP_Sound_Load("./Assets/Sound/SFX/Click.wav");
	
}

void Main_Menu_Update()
{
	/*INITIALISE*/
	set_level(); //set to latest unlocked level
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	/*LOGIC*/
	if (CP_Input_MouseClicked()) { // check for mouse input if pressed down, then check if the mouse is within any of the rectangles
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // load base game state
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Level_Select_Init, Level_Select_Update, Level_Select_Exit); // load options state
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 3.f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Options_Init, Options_Update, Options_Exit); // load options state
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 4.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit); // Load Credit page
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 6.f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Sound_Free(&click);
			CP_Sound_Free(&gameMusic);
			CP_Engine_Terminate(); // quit the program
		}
	}

	/*DRAWING*/
	draw_background(); //Clear background and draws background art
	CP_Settings_NoTint(); //Clear Tint

	CP_Settings_TextSize((float)config.settings.resolutionWidth * .15f);
	drawAlignedText(FADEBLACK, CENTER, "Seven11", (float)config.settings.resolutionWidth / 2, (float)config.settings.resolutionHeight / 5);
	drawAlignedText(PLYRGRN, CENTER, "Seven11", ((float)config.settings.resolutionWidth / 2) + .05f * ((float)config.settings.resolutionWidth * .15f), (float)config.settings.resolutionHeight / 5);

	/*BUTTONS*/
	drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width,  buttons.height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 3.f, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 4.5f, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 6.f, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);

	/*TEXT*/
	CP_Settings_TextSize((float)config.settings.resolutionWidth * 0.019f);
	global_level > 1 ? drawAlignedText(WHITE, CENTER, "Continue", buttons.center_x, buttons.center_y) : drawAlignedText(WHITE, CENTER, "Play", buttons.center_x, buttons.center_y);
	drawAlignedText(WHITE, CENTER, "Select Level", buttons.center_x, buttons.center_y + buttons.height * 1.5f);
	drawAlignedText(WHITE, CENTER, "Options", buttons.center_x, buttons.center_y + buttons.height * 3.f);
	drawAlignedText(WHITE, CENTER, "Credits", buttons.center_x, buttons.center_y + buttons.height * 4.5f);
	drawAlignedText(WHITE, CENTER, "Quit", buttons.center_x, buttons.center_y + buttons.height * 6.f);
}

void Main_Menu_Exit()
{
	free_background();
}
