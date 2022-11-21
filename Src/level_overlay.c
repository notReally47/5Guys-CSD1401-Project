#pragma once
/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
		  Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
			- Level Complete SFX
			- Reason for failure
File	: level_overlay.c
Purpose	: Render Different OVerlay & implement function/purpose of overlay
*/

#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for IsAreaClicked() Function
#include "level_logic.h"	// Needed for global extern variable 'global_level'
#include "mainmenu.h"		// Needed to transition back to Main Menu
#include "easydraw.h"		// Needed for Customised Buttons & Texts
#include "settings.h"		// Needed to write latest level played to file

extern Config config;		// For Resolution Settings
CP_Sound click;				// Clicking SFX

/* Struct for Asset Properties */
typedef struct Size {
	// Header Text Properties
	float header_text_size;
	float header_text_width;
	float header_text_height;

	// Button Text Size
	float button_text_size;

	//Button 1 & Text Properties
	float button_01_position_x;
	float button_01_position_y;
	float button_01_width;
	float button_01_height;

	// Button 2 & Text Properties
	float button_02_position_x;
	float button_02_position_y;
	float button_02_width;
	float button_02_height;

	// Text Box Properties
	float text_box_text_size;
	float text_box_position_x;
	float text_box_position_y;
	float text_box_row_width;
} Size;


/* Initialise Size struct with values that scale with resolution for Pause & Reset Overlay */
struct Size initialise_pause_reset_size() {
	struct Size size = { 0 };
	size.header_text_size = (float)config.settings.resolutionHeight * 0.3f;
	size.header_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.header_text_height = (float)config.settings.resolutionHeight * 0.45f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.button_01_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.button_01_position_y = (float)config.settings.resolutionHeight * 0.6f;
	size.button_01_width = (float)config.settings.resolutionWidth / 6.f;
	size.button_01_height = (float)config.settings.resolutionHeight / 10.f;

	size.button_02_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.button_02_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.button_02_width = (float)config.settings.resolutionWidth / 6.f;
	size.button_02_height = (float)config.settings.resolutionHeight / 10.f;

	size.text_box_text_size = 0.f;
	size.text_box_position_x = 0.f;
	size.text_box_position_y = 0.f;
	size.text_box_row_width = 0.f;

	struct Size set_size = { size.header_text_size, size.header_text_width, size.header_text_height, size.button_text_size, size.button_01_position_x,
		size.button_01_position_y, size.button_01_width, size.button_01_height, size.button_02_position_x, size.button_02_position_y, size.button_02_width, size.button_02_height,
		size.text_box_text_size, size.text_box_position_x, size.text_box_position_y, size.text_box_row_width };

	return set_size;
}

/* Render Pause Overlay */
void overlay_pause() {
	struct Size size = initialise_pause_reset_size();																// Initialise size struct from initialise_pause_rest_size

	CP_Settings_NoTint();																							// Clear any Existing Tint
	
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());									// Set Mouse Poistions into a Vector

	CP_Settings_TextSize(size.header_text_size);																	// Set Header Text Size
	drawAlignedText(WHITE, CENTER, "PAUSED", size.header_text_width, .75f * size.header_text_height);				// Draw Header Text

	drawTintedButton(PLYRRED, size.button_01_position_x, size.button_01_position_y,									// Draw 'Resume' Button
		size.button_01_width, size.button_01_height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, size.button_02_position_x, size.button_02_position_y,									// Draw 'Main Menu' Button			
		size.button_02_width, size.button_02_height, mousePos.x, mousePos.y, NO);

	CP_Settings_TextSize(size.button_text_size);																	// Set Button Text Size
	drawAlignedText(WHITE, CENTER, "RESUME", size.button_01_position_x, size.button_01_position_y);					// Draw 'Resume' Text
	drawAlignedText(WHITE, CENTER, "MAIN MENU", size.button_02_position_x, size.button_02_position_y);				// Draw 'Main Menu' Text
	CP_Settings_Tint(ABITBLACK);																						// Tint the Overlay Transparent DARKGRAY
}

/* Unpause Logic, whether Resume Game or return to Main Menu */
int unpause(int game_pause) {
	struct Size size = initialise_pause_reset_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.button_01_position_x, size.button_01_position_y, size.button_01_width, size.button_01_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);											// Play Clicking SFX
			game_pause = 0;																							// Resume Game is Resume Button Clicked
		}
		else if (IsAreaClicked(size.button_02_position_x, size.button_02_position_y, size.button_02_width, size.button_02_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);											// Play Clicking SFX
			game_pause = 0;
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);							// Load Main Menu when MAIN MENU is Clicked
		}
	}
	return game_pause;
}

/* Initialise Size struct with values that scale with resolution for Game Over Overlay */
struct Size initialise_game_over_size() {
	struct Size size = { 0 };
	size.header_text_size = (float)config.settings.resolutionHeight * 0.2f;
	size.header_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.header_text_height = (float)config.settings.resolutionHeight * 0.45f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.button_01_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.button_01_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.button_01_width = (float)config.settings.resolutionWidth / 6.f;
	size.button_01_height = (float)config.settings.resolutionHeight / 10.f;

	size.button_02_position_x = 0.f;
	size.button_02_position_y = 0.f;
	size.button_02_width = 0.f;
	size.button_02_height = 0.f;

	size.text_box_text_size = (float)config.settings.resolutionHeight * 0.04f;
	size.text_box_position_x = 0.f;
	size.text_box_position_y = 0.f;
	size.text_box_row_width = 0.f;

	struct Size set_size = { size.header_text_size, size.header_text_width, size.header_text_height, size.button_text_size, size.button_01_position_x,
		size.button_01_position_y, size.button_01_width, size.button_01_height, size.button_02_position_x, size.button_02_position_y, size.button_02_width, size.button_02_height,
		size.text_box_text_size, size.text_box_position_x, size.text_box_position_y, size.text_box_row_width };

	return set_size;
}

/* Renders Game_Over overlay */
void overlay_game_over(int gameover) {
	struct Size size = initialise_game_over_size();																	// Initialise size Struct with initialise_game_over_size

	char* fail_text[] = {																							// Different Game Over Texts
		"You were too slow in completing your tasks...",
		"Why do you insist on speaking with every customer???",
		"Why do you just keep wandering around..."
	};

	CP_Settings_NoTint();																							// Clear any Existing Tint
	
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());									// Set Mouse Position into a Vector
	CP_Settings_TextSize(size.header_text_size);																	// Set Header Text Size
	drawAlignedText(WHITE, CENTER, "YOU'RE FIRED!", size.header_text_width, .75f * size.header_text_height);		// Draw Header Text
	
	CP_Settings_TextSize(size.text_box_text_size);																	
	drawAlignedText(WHITE, CENTER, fail_text[gameover], size.header_text_width, 1.f * size.header_text_height);		// Draw Reason for failure

	drawTintedButton(PLYRRED, size.button_01_position_x, size.button_01_position_y,									// Draw 'Main Menu' Button
		size.button_01_width, size.button_01_height, mousePos.x, mousePos.y, NO);

	CP_Settings_TextSize(size.button_text_size);																	// Set Button Text Size
	drawAlignedText(WHITE, CENTER, "MAIN MENU", size.button_01_position_x, size.button_01_position_y);				// Draw 'Main Menu' Text
	
	CP_Settings_Tint(DARKGRAY);																						// Tint the Overlay Transparent DARKGRAY
}

/* Game Over Logic, returns to Main Menu only */
int game_over(int game_pause) {
	struct Size size = initialise_game_over_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.button_01_position_x, size.button_01_position_y, size.button_01_width, size.button_01_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			reset_level();																							// Set Level to 1 when Game Over
			config.save.lastLevelPlayed = global_level;																// Set config.save.lastLevelPlayed to 1	
			writeConfig(config);																					// Save config.dat
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);											// Play Clicking SFX
			game_pause = 0;																							// Set to Unpause
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);							// Load Main Menu when MAIN MENU is Clicked
		}
	}
	return game_pause;
}

/* Render Reset Overlay */
void overlay_reset() {
	struct Size size = initialise_pause_reset_size();																// Initialise size Struct with initialise_pause_reset_size

	CP_Settings_NoTint();																							// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																						// Set Font to WHITE
		
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());									// Set Mouse Position into a Vector
	CP_Settings_TextSize(size.header_text_size);																	// Set Header Text Size
	drawAlignedText(WHITE, CENTER, "RESET MAP?", size.header_text_width, .75f * size.header_text_height);			// Draw Header Text
	
	drawTintedButton(PLYRRED, size.button_01_position_x, size.button_01_position_y,									// Draw 'Yes' Button
		size.button_01_width, size.button_01_height, mousePos.x, mousePos.y, NO);
	drawTintedButton(PLYRRED, size.button_02_position_x, size.button_02_position_y,									// Draw 'No' Button
		size.button_02_width, size.button_02_height, mousePos.x, mousePos.y, NO);

	CP_Settings_TextSize(size.button_text_size);																	// Set Button Text Size
	drawAlignedText(WHITE, CENTER, "YES	", size.button_01_position_x, size.button_01_position_y);					// Draw 'Yes' Text
	drawAlignedText(WHITE, CENTER, "NO", size.button_02_position_x, size.button_02_position_y);						// Draw 'No' Text
	CP_Settings_Tint(ABITBLACK);																						// Tint the Overlay Transparent DARKGRAY
}

/* Reset Map if Yes, return to Game if No */
int reset_check(int reset_confirmed) {
	struct Size size = initialise_pause_reset_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.button_01_position_x, size.button_01_position_y, size.button_01_width, size.button_01_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);											// Play Click Sound
			reset_confirmed = 1;																					// Confirm Reset													
		}
		else if (IsAreaClicked(size.button_02_position_x, size.button_02_position_y, size.button_02_width, size.button_02_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);											// Play Click Sound
			reset_confirmed = 2;																					// Reset Cancelled
		}
	}
	return reset_confirmed;
}

/* Initialise Size struct with values that scale with resolution for Welcome & Game End Overlay */
struct Size initialise_welcome_game_end_size() {
	struct Size size = { 0 };
	size.header_text_size = (float)config.settings.resolutionHeight * 0.1f;
	size.header_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.header_text_height = (float)config.settings.resolutionHeight * 0.15f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.button_01_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.button_01_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.button_01_width = (float)config.settings.resolutionWidth / 6.f;
	size.button_01_height = (float)config.settings.resolutionHeight / 10.f;

	size.button_02_position_x = 0.f;
	size.button_02_position_y = 0.f;
	size.button_02_width = 0.f;
	size.button_02_height = 0.f;

	size.text_box_text_size = (float)config.settings.resolutionHeight * 0.032f;
	size.text_box_position_x = (float)config.settings.resolutionWidth / 4.f;
	size.text_box_position_y = (float)config.settings.resolutionHeight * 0.2f;
	size.text_box_row_width = (float)config.settings.resolutionWidth / 1.90f;

	struct Size set_size = { size.header_text_size, size.header_text_width, size.header_text_height, size.button_text_size, size.button_01_position_x,
		size.button_01_position_y, size.button_01_width, size.button_01_height, size.button_02_position_x, size.button_02_position_y, size.button_02_width, size.button_02_height,
		size.text_box_text_size, size.text_box_position_x, size.text_box_position_y, size.text_box_row_width };

	return set_size;
}

/* Renders Welcome Message Overlay */
void overlay_welcome() {
	struct Size size = initialise_welcome_game_end_size();
	char* welcome_text = "Welcome uhh... New Guy, to Seven11! Today will be your first day on the job & we will be watching your every move."
		" Your job is to move erm... Stuff, around to the designated place & avoid talking to the customers. However, Customers will take every chance they get to talk to you."
		" If you waste too much time or talk to too many customers, we're afraid we will have to let you go. Yes, you can be fired on your first day!"
		" So get out there and only move them things. You got everything? Alright, nice talk. Have a pleasant working experience with us.";

	char* move_text = "Uhh.. You do know how to move right? Well, if you didn't, and I'm not sure why, use the W, A, S, D keys. We have also given you the Retrace Stone together with your employee handbook."
		" Simply press U to undo a move & R to reset everything. Be careful as undoing a move will not recover lost time! Now Then, Happy Working!";

	
	CP_Settings_NoTint();																										// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
		
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());												// Set Mouse Position into a Vector
	CP_Settings_TextSize(size.header_text_size);																				// Set Header Text Size
	drawAlignedText(WHITE, CENTER, "WELCOME!", size.header_text_width, .75f * size.header_text_height);							// Draw Header Text

	drawTintedButton(PLYRRED, size.button_01_position_x, size.button_01_position_y,												// Draw 'Let's Work!' Button		
		size.button_01_width, size.button_01_height, mousePos.x, mousePos.y, NO);

	CP_Settings_TextSize(size.button_text_size);																				// Set Button Text Size
	drawAlignedText(WHITE, CENTER, "Let's Work", size.button_01_position_x, size.button_01_position_y);							// Draw 'Let's Work!' Text
	
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Center
	CP_Settings_TextSize(size.text_box_text_size);																				// Set Welcome Message Text Size
	CP_Font_DrawTextBox(welcome_text, size.text_box_position_x, size.text_box_position_y, size.text_box_row_width);				// Draw Welcome Message
	CP_Font_DrawTextBox(move_text, size.text_box_position_x, size.text_box_position_y * 2.5f, size.text_box_row_width);			// Draw Welcome Message Cont'd
	
	CP_Settings_Tint(ABITBLACK);																								// Tint the Overlay Transparent DARKGRAY
}

/* Remove Welcome Message Overlay & allow Gameplay */
int welcome_done(int game_pause) {
	struct Size size = initialise_welcome_game_end_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.button_01_position_x, size.button_01_position_y, size.button_01_width, size.button_01_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, .5f, 2, FALSE, CP_SOUND_GROUP_SFX);													// Play Click Sound
			game_pause = 0;																										// Unpause & Start Game
		}
	}
	return game_pause;
}

/* Renders Game End Overlay */
void overlay_end_game(){
	struct Size size = initialise_welcome_game_end_size();
	char* game_end_text = "Business has not been so good lately & we're afraid we will have to close down the shop."
		" You have done a... Great job."
		" Who knows maybe we will have a chance to work again in the future...";

	char* thank_you_text = "Thanks For Playing, You can Replay the levels and choose different card effects for the levels!";

	CP_Settings_NoTint();																										// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																									// Set Font to WHITE

	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());												// Set Mouse Position into a Vector
	CP_Settings_TextSize(size.header_text_size * 2.0f);																			// Set Header Text Size
	drawAlignedText(WHITE, CENTER, "THE END!", size.header_text_width, .75f * size.header_text_height*1.0f);					// Draw Header Text

	drawTintedButton(PLYRRED, size.button_01_position_x, size.button_01_position_y,												// Draw 'Oh Well...' Button		
		size.button_01_width, size.button_01_height, mousePos.x, mousePos.y, NO);

	CP_Settings_TextSize(size.button_text_size);																				// Set Button Text Size
	drawAlignedText(WHITE, CENTER, "Oh Well...", size.button_01_position_x, size.button_01_position_y);							// Draw 'Oh Well...' Text
	

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Center
	CP_Settings_TextSize(size.text_box_text_size);																				// Set Welcome Message Text Size
	CP_Font_DrawTextBox(game_end_text, size.text_box_position_x, size.text_box_position_y * 1.5f, size.text_box_row_width);		// Draw Game End Message
	CP_Font_DrawTextBox(thank_you_text, size.text_box_position_x, size.text_box_position_y * 2.5f, size.text_box_row_width);	// Draw Thank You Message

	CP_Settings_Tint(ABITBLACK);																								// Tint Game Translucent Black
}

/* Return to Main Menu */
void end_game(){
	struct Size size = initialise_welcome_game_end_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.button_01_position_x, size.button_01_position_y, size.button_01_width, size.button_01_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Sound_PlayAdvanced(click, .5f, 2, FALSE, CP_SOUND_GROUP_SFX);													// Play Click Sound
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);										// Load Main Menu when Button is Clicked
		}
	}

}