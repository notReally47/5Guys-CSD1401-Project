#include "cprocessing.h"
#include "utils.h"
#include "credits.h"
#include "defines.h"
#include "basegame.h"
#include "options.h"
#include "level_logic.h"
#include "level_select.h"
#include "easydraw.h"

extern Config config;
float windowwidth, windowheight;
CP_Sound click = NULL;

rect buttons;

void Main_Menu_Init()
{
	// declare/define window width/height
	windowwidth = (float)CP_System_GetWindowWidth();
	windowheight = (float)CP_System_GetWindowHeight();
	CP_Settings_Stroke(BLACK);
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	// define buttons
	buttons.center_x = windowwidth * 0.5f;
	buttons.center_y = windowheight * 0.4f;
	buttons.width = windowwidth * 0.15f;
	buttons.height = windowheight * 1.f / 12.f;

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
			CP_Engine_Terminate(); // quit the program
		}
	}

	/*DRAWING*/
	CP_Settings_NoTint(); //Clear Tint
	CP_Graphics_ClearBackground(GRAY); // set background to dark grey

	CP_Settings_TextSize(windowwidth * .15f);
	drawAlignedText(WHITE, CENTER, "Seven11", windowwidth / 2, windowheight / 5);

	/*BUTTONS*/
	drawTintedButton(RED, buttons.center_x, buttons.center_y, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);
	drawTintedButton(RED, buttons.center_x, buttons.center_y + buttons.height * 1.5f, buttons.width,  buttons.height, mousePos.x, mousePos.y, YES);
	drawTintedButton(RED, buttons.center_x, buttons.center_y + buttons.height * 3.f, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);
	drawTintedButton(RED, buttons.center_x, buttons.center_y + buttons.height * 4.5f, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);
	drawTintedButton(RED, buttons.center_x, buttons.center_y + buttons.height * 6.f, buttons.width, buttons.height, mousePos.x, mousePos.y, YES);

	/*TEXT*/
	CP_Settings_TextSize(windowwidth * 0.025f);
	global_level > 1 ? drawAlignedText(WHITE, CENTER, "Continue", buttons.center_x, buttons.center_y) : drawAlignedText(WHITE, CENTER, "Play", buttons.center_x, buttons.center_y);
	drawAlignedText(WHITE, CENTER, "Select Level", buttons.center_x, buttons.center_y + buttons.height * 1.5f);
	drawAlignedText(WHITE, CENTER, "Options", buttons.center_x, buttons.center_y + buttons.height * 3.f);
	drawAlignedText(WHITE, CENTER, "Credits", buttons.center_x, buttons.center_y + buttons.height * 4.5f);
	drawAlignedText(WHITE, CENTER, "Quit", buttons.center_x, buttons.center_y + buttons.height * 6.f);
}

void Main_Menu_Exit()
{

}
