#include "cprocessing.h"	//Needed for C Processing Functions
#include "utils.h"			//Needed for Mouse Click Logic
#include "defines.h"		// Needed for define Values
#include "mainmenu.h"		// Needed to Return to Main Menu
#include "basegame.h"		// Needed to Transit to Next Level

extern Config config;
float windowwidth, windowheight;
rect buttons;

void Level_Transition_Init()
{
	// declare/define window width/height
	CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
	windowwidth = (float)CP_System_GetWindowWidth();
	windowheight = (float)CP_System_GetWindowHeight();
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255)); // black border around the rectangle
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // align text to the middle of the rect box

	// define buttons
	buttons.center_x = windowwidth * 0.5f;
	buttons.center_y = windowheight * 0.5f;
	buttons.width = windowwidth * 0.1f;
	buttons.height = windowheight * 1.f / 12.f;
}

void Level_Transition_Update()
{
	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_Fill(RED); // Fill Rectangle RED
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y, buttons.width, buttons.height); // Draw 1st Button as 'Next'
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height); // Draw 2nd Button as 'Main Menu'
	CP_Settings_Fill(BLACK); // Set font to BLACK

	// Draw Texts 'Next' & 'Main Menu' on Rectangles
	CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f); // Set Text Size scaling to Window Height
	CP_Font_DrawText("Next", buttons.center_x, buttons.center_y);
	CP_Font_DrawText("Main Menu", buttons.center_x, buttons.center_y + buttons.height * 1.5f);
	CP_Settings_Fill(WHITE); // Set font to WHITE

	// Draw Text 'Level Complete!"
	CP_Settings_TextSize(100.f);
	CP_Font_DrawText("Level Complete!", (float)CP_System_GetWindowWidth() / 2, 150.f);

	// Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
	if (CP_Input_MouseClicked()) { 
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Go to the Next Level
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); // Return to Main Menu
		}
	}
}

void Level_Transition_Exit()
{

}
