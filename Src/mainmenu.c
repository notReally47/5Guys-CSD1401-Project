#include "cprocessing.h"
#include "utils.h"

struct rect { // declare struct variable rect for x/y coords of a rectangle, width and height
	float center_x,center_y,width,height;
} buttons; // declare variable buttons for 'play', 'options' and 'exit'

void Main_Menu_Init()
{	
	CP_Graphics_ClearBackground(CP_Color_Create(80,80,80,255)); // set background to dark grey
	// declare/define window width/height
	float windowwidth = (float)CP_System_GetWindowWidth();
	float windowheight = (float)CP_System_GetWindowHeight();
	CP_Settings_RectMode(CP_POSITION_CENTER); // align rectangle to the center position (else it defaults to top left corner)
	CP_Settings_Stroke(CP_Color_Create(0,0,0,255)); // black border around the rectangle
	CP_Settings_TextSize(windowheight*0.025f); // set text size scaling to window height
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER,CP_TEXT_ALIGN_V_MIDDLE); // align text to the middle of the rect box
	// define buttons
	buttons.center_x = windowwidth*0.5f;
	buttons.center_y = windowheight*0.5f;
	buttons.width = windowwidth*0.1f;
	buttons.height = windowheight*1.f/12.f;
}

void Main_Menu_Update()
{
	CP_Settings_Fill(CP_Color_Create(200,50,50,255)); // fill rect with the colour red
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y,buttons.width,buttons.height); // draw buttons as 'play'
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y+buttons.height*1.5f,buttons.width,buttons.height); // draw buttons as options'
	CP_Graphics_DrawRect(buttons.center_x,buttons.center_y+buttons.height*3.f,buttons.width,buttons.height); // draw r_exit as 'exit'
	CP_Settings_Fill(CP_Color_Create(0,0,0,255)); // set font to black
	// draw text 'play', 'options' and 'exit' on their respective boxes
	CP_Font_DrawText("Play",buttons.center_x,buttons.center_y);
	CP_Font_DrawText("Options", buttons.center_x,buttons.center_y+buttons.height*1.5f);
	CP_Font_DrawText("Exit",buttons.center_x,buttons.center_y+buttons.height*3.f);
	
	if (CP_Input_MouseClicked()) { // check for mouse input if pressed down, then check if the mouse is within any of the rectangles
		if (IsAreaClicked(buttons.center_x,buttons.center_y,buttons.width,buttons.height,CP_Input_GetMouseX(),CP_Input_GetMouseY())) {
			//CP_Engine_SetNextGameState(Level_Init,Level_Update,Leve_Exit); // load level state
		}
		else if (IsAreaClicked(buttons.center_x,buttons.center_y+buttons.height*1.5f,buttons.width,buttons.height,CP_Input_GetMouseX(),CP_Input_GetMouseY())) {
			//CP_Engine_SetNextGameState(Level_Init,Level_Update,Leve_Exit); // load options state
		}
		else if (IsAreaClicked(buttons.center_x,buttons.center_y+buttons.height*3.f,buttons.width,buttons.height,CP_Input_GetMouseX(),CP_Input_GetMouseY())) {
			CP_Engine_Terminate(); // quit the program
		}
	}		
}

void Main_Menu_Exit()
{

}