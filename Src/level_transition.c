#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for Mouse Click Logic
#include "defines.h"		// Needed for define Values
#include "mainmenu.h"		// Needed to Return to Main Menu
#include "basegame.h"		// Needed to Transit to Next Level
#include "mechanics.h"		// Needed for card_selection()
#include "level_logic.h"	// Needed for level global variable
#include "spritesheet.h"

extern Config config;
rect buttons;
int selected;
CP_Sound nextLvl = NULL, click;

void Level_Transition_Init()
{
	// declare/define window width/height
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255)); // black border around the rectangle
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // align text to the middle of the rect box

	// Define buttons
	buttons.center_x = (float)config.settings.resolutionWidth * 0.5f;
	buttons.center_y = (float)config.settings.resolutionHeight * 0.5f;
	buttons.width = (float)config.settings.resolutionWidth * 0.1f;
	buttons.height = (float)config.settings.resolutionHeight * 1.f / 12.f;

	selected = 0;

	// Level Complete SFX
	nextLvl = CP_Sound_Load("./Assets/Sound/SFX/Level.wav");
	CP_Sound_PlayAdvanced(nextLvl, 1, 1, FALSE, CP_SOUND_GROUP_SFX);

	// Load background art
	load_background();
}

void Level_Transition_Update()
{
	draw_background(); // clears and draws background

	for (int i = 2; i < 10; i++) {
		if (global_level == i) {
			if (!(UM.selectedflag & 2 << (i - 2)))
				card_selection(1, &selected);
			else
				selected = 1;
		}
	}
	if (global_level == 1 || global_level == 10)
		selected = 1;
	
	(selected == 0) ? CP_Settings_Fill(GRAY) : CP_Settings_Fill(RED); // Fill Rectangle RED
	if (CP_Input_MouseClicked() && selected == 1) { // Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Go to the Next Level
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); // Return to Main Menu
		}
	}
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y, buttons.width, buttons.height);							// Draw 1st Button as 'Next'
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height);	// Draw 2nd Button as 'Main Menu'
	CP_Settings_Fill(BLACK);																							// Set Font to BLACK

	// Draw Texts 'Next' & 'Main Menu' on Rectangles
	CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f);															// Set Text Size scaling to Window Height
	CP_Font_DrawText("Next", buttons.center_x, buttons.center_y);
	CP_Font_DrawText("Main Menu", buttons.center_x, buttons.center_y + buttons.height * 1.5f);
	CP_Settings_Fill(WHITE);																							// Set Font to WHITE

	// Draw Text 'Level Complete!"
	CP_Settings_TextSize(100.f);
	CP_Font_DrawText("Level Complete!", (float)CP_System_GetWindowWidth() / 2, 150.f);
}

void Level_Transition_Exit()
{
	free_background();
	CP_Sound_Free(&nextLvl);
}
