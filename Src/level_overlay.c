#include "cprocessing.h"	// Needed for C Processing Functions
#include "utils.h"			// Needed for IsAreaClicked() Function
#include "level_logic.h"	// Needed for global extern variable 'global_level'
#include "mainmenu.h"		// Needed to transition back to Main Menu

extern Config config;		// For Resolution Settings

/* Struct for Pause Overlay Asset Properties */
typedef struct Pause_Size {
	// Pause Text Properties
	float pause_text_size;
	float pause_text_width;
	float pause_text_height;

	// Button Text Size
	float button_text_size;

	// Resume Button & Text Properties
	float resume_position_x;
	float resume_position_y;
	float resume_width;
	float resume_height;

	// Main Menu Button & Text Properties
	float main_menu_position_x;
	float main_menu_position_y;
	float main_menu_width;
	float main_menu_height;
} Pause_Size;

/* Struct for Game Over Overlay Asset Properties */
typedef struct Game_Over_Size {
	// Game Over Text Properties
	float game_over_text_size;
	float game_over_text_width;
	float game_over_text_height;

	// Button Text Size
	float button_text_size;

	// Main Menu Button & Text Size Properties
	float main_menu_position_x;
	float main_menu_position_y;
	float main_menu_width;
	float main_menu_height;
} Game_Over_Size;

typedef struct Reset_Size {
	// Pause Text Properties
	float reset_text_size;
	float reset_text_width;
	float reset_text_height;

	// Button Text Size
	float button_text_size;

	// Yes Button & Text Properties
	float yes_position_x;
	float yes_position_y;
	float yes_width;
	float yes_height;

	// No Button & Text Properties
	float no_position_x;
	float no_position_y;
	float no_width;
	float no_height;
} Reset_Size;

/* Initialise Pause_Size struct with values that scale with resolution */
struct Pause_Size initialise_pause_size() {
	struct Pause_Size size;
	size.pause_text_size = (float)config.settings.resolutionHeight * 0.3f;
	size.pause_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.pause_text_height = (float)config.settings.resolutionHeight * 0.45f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.resume_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.resume_position_y = (float)config.settings.resolutionHeight * 0.6f;
	size.resume_width = (float)config.settings.resolutionWidth / 6.f;
	size.resume_height = (float)config.settings.resolutionHeight / 10.f;

	size.main_menu_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.main_menu_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.main_menu_width = (float)config.settings.resolutionWidth / 6.f;
	size.main_menu_height = (float)config.settings.resolutionHeight / 10.f;

	struct Pause_Size set_size = { size.pause_text_size, size.pause_text_width, size.pause_text_height, size.button_text_size, size.resume_position_x, 
		size.resume_position_y, size.resume_width, size.resume_height, size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height };

	return set_size;
}

/* Render Pause Overlay */
void overlay_pause() {
	struct Pause_Size size = initialise_pause_size();

	CP_Settings_NoTint();																										// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Middle
	CP_Settings_TextSize(size.pause_text_size);																					// Set Font Size
	CP_Font_DrawText("PAUSE", size.pause_text_width, size.pause_text_height);													// Draw PAUSE Text

	CP_Settings_Fill(RED);																										// Set Button Colour to RED
	CP_Graphics_DrawRect(size.resume_position_x, size.resume_position_y, size.resume_width, size.resume_height);				// Place Holder for 'Resume' Buttom
	CP_Graphics_DrawRect(size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height);	// Place Holder for 'Main Menu' Button

	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextSize(size.button_text_size);																				// Set Font Size
	CP_Font_DrawText("RESUME", size.resume_position_x, size.resume_position_y);													// Draw RESUME Text
	CP_Font_DrawText("MAIN MENU", size.main_menu_position_x, size.main_menu_position_y);										// Draw MAIN MENUU Text
	CP_Settings_Tint(DARKGRAY);																									// Tint the Overlay Transparent DARKGRAY
}

/* Unpause Logic, whether Resume Game or return to Main Menu */
int unpause(int game_pause) {
	struct Pause_Size size = initialise_pause_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.resume_position_x, size.resume_position_y, size.resume_width, size.resume_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			game_pause = 0;																										// Resume Game is Resume Button Clicked
		}
		else if (IsAreaClicked(size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			game_pause = 0;
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);										// Load Main Menu when MAIN MENU is Clicked
		}
	}
	return game_pause;
}

/* Initialise Game_Over_Size structwith values that scale with resolution */
struct Game_Over_Size initialise_game_over_size() {
	struct Game_Over_Size size;
	size.game_over_text_size = (float)config.settings.resolutionHeight * 0.3f;
	size.game_over_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.game_over_text_height = (float)config.settings.resolutionHeight * 0.45f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.main_menu_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.main_menu_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.main_menu_width = (float)config.settings.resolutionWidth / 6.f;
	size.main_menu_height = (float)config.settings.resolutionHeight / 10.f;

	struct Game_Over_Size set_size = { size.game_over_text_size, size.game_over_text_width, size.game_over_text_height, 
		size.button_text_size, size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height };

	return set_size;
}

/* Renders Game_Over overlay */
void overlay_game_over() {
	struct Game_Over_Size size = initialise_game_over_size();

	CP_Settings_NoTint();																										// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Middle
	CP_Settings_TextSize(size.game_over_text_size);																				// Set Font Size
	CP_Font_DrawText("YOU'RE FIRED!", size.game_over_text_width, size.game_over_text_height);									// Draw "YOU'RE FIRED!" Text

	CP_Settings_Fill(RED);																										// Set Button Colour to RED
	CP_Graphics_DrawRect(size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height);	// Place Holder for 'Main Menu' Button

	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextSize(size.button_text_size);																				// Set Font Size
	CP_Font_DrawText("MAIN MENU", size.main_menu_position_x, size.main_menu_position_y);										// Draw MAIN MENU Text
	CP_Settings_Tint(DARKGRAY);																									// Tint the Overlay Transparent DARKGRAY
}

/* Game Over Logic, returns to Main Menu only */
int game_over(int game_pause) {
	global_level = 1;
	struct Game_Over_Size size = initialise_game_over_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			game_pause = 0;
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);										// Load Main Menu when MAIN MENU is Clicked
		}
	}
	return game_pause;
}

/* Initialise Reset_Size struct with values that scale with resolution */
struct Reset_Size initialise_reset_size() {
	struct Reset_Size size;
	size.reset_text_size = (float)config.settings.resolutionHeight * 0.3f;
	size.reset_text_width = (float)config.settings.resolutionWidth / 2.f;
	size.reset_text_height = (float)config.settings.resolutionHeight * 0.45f;

	size.button_text_size = (float)config.settings.resolutionHeight * 0.04f;

	size.yes_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.yes_position_y = (float)config.settings.resolutionHeight * 0.6f;
	size.yes_width = (float)config.settings.resolutionWidth / 6.f;
	size.yes_height = (float)config.settings.resolutionHeight / 10.f;

	size.no_position_x = (float)config.settings.resolutionWidth / 2.f;
	size.no_position_y = (float)config.settings.resolutionHeight * 0.75f;
	size.no_width = (float)config.settings.resolutionWidth / 6.f;
	size.no_height = (float)config.settings.resolutionHeight / 10.f;

	struct Reset_Size set_size = { size.reset_text_size, size.reset_text_width, size.reset_text_height, size.button_text_size, size.yes_position_x,
		size.yes_position_y, size.yes_width, size.yes_height, size.no_position_x, size.no_position_y, size.no_width, size.no_height };

	return set_size;
}

/* Render Reset Overlay */
void overlay_reset() {
	struct Reset_Size size = initialise_reset_size();

	CP_Settings_NoTint();																										// Clear any Existing Tint
	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Middle
	CP_Settings_TextSize(size.reset_text_size);																					// Set Font Size
	CP_Font_DrawText("RESET MAP?", size.reset_text_width, size.reset_text_height);												// Draw RESET MAP? Text

	CP_Settings_Fill(RED);																										// Set Button Colour to RED
	CP_Graphics_DrawRect(size.yes_position_x, size.yes_position_y, size.yes_width, size.yes_height);							// Place Holder for 'YES' Buttom
	CP_Graphics_DrawRect(size.no_position_x, size.no_position_y, size.no_width, size.no_height);								// Place Holder for 'NO' Button

	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextSize(size.button_text_size);																				// Set Font Size
	CP_Font_DrawText("YES", size.yes_position_x, size.yes_position_y);															// Draw YES Text
	CP_Font_DrawText("NO", size.no_position_x, size.no_position_y);																// Draw NO Text
	CP_Settings_Tint(DARKGRAY);																									// Tint the Overlay Transparent DARKGRAY
}

/* Reset Map if Yes, return to Game if No */
int reset_check(int reset_confirmed) {
	struct Reset_Size size = initialise_reset_size();

	/* Check for Mouse Click Input */
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(size.yes_position_x, size.yes_position_y, size.yes_width, size.yes_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			reset_confirmed = 1;																								// Returns 1 to Reset Map																								
		}
		else if (IsAreaClicked(size.no_position_x, size.no_position_y, size.no_width, size.no_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			reset_confirmed = 2;																								// Returns 2 to Return to Game
		}
	}
	return reset_confirmed;
}