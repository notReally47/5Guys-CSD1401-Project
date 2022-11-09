#include "cprocessing.h"	// Needed for C Processing Functions
#include "defines.h"		// Needed for defined values
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

	CP_Settings_NoTint();
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
	CP_Settings_Tint(DARKGRAY);
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

	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);													// Align Text to Middle
	CP_Settings_TextSize(size.game_over_text_size);																				//Set Font Size
	CP_Font_DrawText("YOU'RE FIRED!", size.game_over_text_width, size.game_over_text_height);									// Draw "YOU'RE FIRED!" Text

	CP_Settings_Fill(RED);																										// Set Button Colour to RED
	CP_Graphics_DrawRect(size.main_menu_position_x, size.main_menu_position_y, size.main_menu_width, size.main_menu_height);	// Place Holder for 'Main Menu' Button

	CP_Settings_Fill(WHITE);																									// Set Font to WHITE
	CP_Settings_TextSize(size.button_text_size);																				// Set FONT Size
	CP_Font_DrawText("MAIN MENU", size.main_menu_position_x, size.main_menu_position_y);										// Draw MAIN MENU Text
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

/* Free PNG file */
//void free_overlay() {
//	black_film = CP_Image_Load("Assets/BLACK_FILM.png");
//	CP_Image_Free(&black_film);
//}