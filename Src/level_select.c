#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for Mouse Click Logic
#include "defines.h"		// Needed for define Values
#include "mainmenu.h"		// Needed to Return to Main Menu
#include "level_logic.h"	// Needed to use Global Extern Variable 'level'
#include "basegame.h"		// Needed to transit into Game Levels

extern Config config;
float windowwidth, windowheight;
rect buttons;

void Level_Select_Init()
{
	//// declare/define window width/height
	//CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
	//windowwidth = (float)CP_System_GetWindowWidth();
	//windowheight = (float)CP_System_GetWindowHeight();
	//CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	//CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255)); // black border around the rectangle
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // align text to the middle of the rect box

	// define buttons
	buttons.center_x = windowwidth * 0.5f;
	buttons.center_y = windowheight * 0.5f;
	buttons.width = windowwidth * 0.1f;
	buttons.height = windowheight * 1.f / 12.f;
}

void Level_Select_Update()
{
	
	float height = 0.f;
	char level_char[2] = {'\0'};

	CP_Graphics_ClearBackground(GRAY);

	/* For-Loop to Draw Rectangles/Buttons for multiple Levels */
	for (int i = 0; i < 5; i++, height += 1.5f) {
		if ((i + 1) <= global_level) {
			CP_Settings_Fill(RED); // Fill Rectangle RED
		}
		else {
			CP_Settings_Fill(GRAY); // Fill Rectangle GRAY
		}
		
		CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * height, buttons.width, buttons.height); // Draw Rrectangles as Level Buttons
		CP_Settings_Fill(BLACK); // Set font to BLACK

		level_char[0] = (i + 1) + '0'; // To be set as Text for Level Number
		CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f); // Set Text Size scaling to Window Height
		CP_Font_DrawText(level_char, buttons.center_x, buttons.center_y + buttons.height * height); // Draws Level Number Text
	}
	
	// Header Text
	CP_Settings_Fill(WHITE); // Set font to WHITE
	CP_Settings_TextSize(100.f);
	CP_Font_DrawText("Select Level", (float)CP_System_GetWindowWidth() / 2, 150.f); // Draws 'Select Level' Text

	// Check for Mouse Input if Clicked, then checks if the Mouse is within any of the Rectangles
	if (CP_Input_MouseClicked()) {
		height = 0.f;
		/* For-Loop for Rectangle/Button On-Click Function Implementation */
		for (int j = 0; j < 5; j++, height += 1.5f) {
			if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * height, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				if (global_level > j) {
					set_temp_level(j+1); // Send over which level to load
					CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Load Level
				}
			}
		}
	}

	// Returns to Main Menu if Player hits 'ESC'
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

}

void Level_Select_Exit()
{

}
