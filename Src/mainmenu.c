#include "cprocessing.h"
#include "utils.h"
#include "credits.h"
#include "defines.h"
#include "basegame.h"
#include "options.h"
#include "level_logic.h"
#include "level_select.h"

extern Config config;
float windowwidth,windowheight;

rect buttons;

void Main_Menu_Init()
{
	// declare/define window width/height
	//CP_System_SetWindowSize(config.settings.resolutionWidth,config.settings.resolutionHeight);
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

void Main_Menu_Update()
{
	set_level(); //set to latest unlocked level
	CP_Settings_NoTint(); //Clear Tint
	CP_Graphics_ClearBackground(GRAY); // set background to dark grey
	CP_Settings_Fill(RED); // fill rect with the colour red
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y,buttons.width,buttons.height); // draw buttons as 'play'
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height); // draw buttons as continue
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y+buttons.height*3.f,buttons.width,buttons.height); // draw buttons as options'
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y+buttons.height*4.5f,buttons.width,buttons.height); // draw button for Credits
	CP_Graphics_DrawRect(buttons.center_x, buttons.center_y + buttons.height * 6.f, buttons.width, buttons.height); // draw exit button
	CP_Settings_Fill(BLACK); // set font to black
	// draw text 'play', 'options', 'credits' and 'exit' on their respective boxes
	CP_Settings_TextSize(CP_System_GetWindowWidth() * 0.025f); // set text size scaling to window height
	(global_level > 1) ? CP_Font_DrawText("Continue", buttons.center_x, buttons.center_y) : CP_Font_DrawText("Play", buttons.center_x, buttons.center_y); //if player has cleared levels, set to continue to go to latest unlocked level
	CP_Font_DrawText("Select Level", buttons.center_x, buttons.center_y + buttons.height * 1.5f);
	CP_Font_DrawText("Options", buttons.center_x,buttons.center_y+buttons.height*3.f);
	CP_Font_DrawText("Credits",buttons.center_x,buttons.center_y+buttons.height*4.5f);
	CP_Font_DrawText("Quit", buttons.center_x, buttons.center_y + buttons.height * 6.f);
	// Draw game title
	CP_Settings_Fill(WHITE); // set font to WHITE
	CP_Settings_TextSize(100.f);
	CP_Font_DrawText("Seven11",windowwidth/2,windowheight/4);

	if (CP_Input_MouseClicked()) { // check for mouse input if pressed down, then check if the mouse is within any of the rectangles
		if (IsAreaClicked(buttons.center_x, buttons.center_y, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(base_Init, base_Update, base_Exit); // Temparary set to launch game_test
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Level_Select_Init, Level_Select_Update, Level_Select_Exit); // load options state
		}
		else if (IsAreaClicked(buttons.center_x,buttons.center_y+buttons.height*3.f,buttons.width,buttons.height,CP_Input_GetMouseX(),CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Options_Init, Options_Update, Options_Exit); // load options state
		}
		else if (IsAreaClicked(buttons.center_x,buttons.center_y+buttons.height*4.5f,buttons.width,buttons.height,CP_Input_GetMouseX(),CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit); // Load Credit page
		}
		else if (IsAreaClicked(buttons.center_x, buttons.center_y + buttons.height * 6.f, buttons.width, buttons.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_Terminate(); // quit the program
		}
	}
}

void Main_Menu_Exit()
{

}
