#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for Mouse Click Logic
#include "defines.h"		// Needed for define Values
#include "mainmenu.h"		// Needed to Return to Main Menu
#include "basegame.h"		// Needed to Transit to Next Level
#include "mechanics.h"		// Needed for card_selection()
#include "level_logic.h"	// Needed for level global variable
#include "spritesheet.h"
#include "easydraw.h"
#include "settings.h"

extern Config config;
rect buttons;
Flag applied;
CP_Sound nextLvl = NULL, click;

void Level_Transition_Init()
{
	// declare/define window width/height
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)

	// Define buttons
	buttons.center_x = (float)config.settings.resolutionWidth * 0.5f;
	buttons.center_y = (float)config.settings.resolutionHeight * 0.5f;
	buttons.width = (float)config.settings.resolutionWidth * 0.1f;
	buttons.height = (float)config.settings.resolutionHeight * 1.f / 12.f;

	applied = NO;

	// Level Complete SFX
	nextLvl = CP_Sound_Load("./Assets/Sound/SFX/Level.wav");
	CP_Sound_PlayAdvanced(nextLvl, 1, 1, FALSE, CP_SOUND_GROUP_SFX);

	// Load background art
	load_background();
	card_settings();
}

void Level_Transition_Update()
{
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	//CP_Color btnColor = selected ? PLYRRED : GRAY;
	CP_Color lvlComplete = !(CP_System_GetFrameCount() % 5) ? PLYRRED : PLYRGRN;	// Change color every 5 frames
	draw_background(); // clears and draws background

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
	
	if (CP_Input_MouseClicked() && applied) { // Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Go to the Next Level
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); // Return to Main Menu
		}
	}
	if (applied) {
		drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);
		drawTintedButton(PLYRRED, buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, mousePos.x, mousePos.y, NO);
	}
	else {
		CP_Settings_Fill(GRAY);
		CP_Settings_StrokeWeight(0.0f);
		CP_Graphics_DrawRect(buttons.center_x, buttons.center_y, buttons.width, buttons.height);
		CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height);
	}

	// Draw Texts 'Next' & 'Main Menu' on Rectangles
	CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f);															// Set Text Size scaling to Window Height
	drawAlignedText(WHITE, CENTER, "Next", buttons.center_x, buttons.center_y);											// Draw 1st Button as 'Next
	drawAlignedText(WHITE, CENTER, "Main Menu", buttons.center_x, buttons.center_y + buttons.height * 1.5f);			// Draw 2nd Button as 'Main Menu'

	// Draw Text 'Level Complete!"
	CP_Settings_TextSize((float)CP_System_GetWindowWidth() * .1f);
	drawAlignedText(lvlComplete, CENTER, "Level Complete!", (float)CP_System_GetWindowWidth() / 2, (float)config.settings.resolutionHeight / 5);
}

void Level_Transition_Exit()
{
	free_buttons();
	free_background();
	CP_Sound_Free(&nextLvl);
}
