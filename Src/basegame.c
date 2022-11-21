/*
Author	: All Members
File	: basegame.c
Purpose	: State for Main Gameplay
*/

#include "cprocessing.h"			// Needed for C Processing Functions
#include "structs.h"				// Needed for Game Asset Structs
#include "customer.h"				// Needed for Customer Logic Function Calls	
#include "utils.h"					// Needed for movement/collision Logic Function Calls
#include "defines.h"				// Needed for Defined Values
#include "settings.h"				// Needed for Read/Write to config.dat
#include "spritesheet.h"			// Needed for Sprite/Art Drawing/Rendering
#include "mechanics.h"				// Needed for Unique Mechanics Function Calls
#include "mainmenu.h"				// Needed to Transit Back to Main Menu if needed
#include "movement.h"				// Needed for save_move() & undo_move() functions
#include "level_logic.h"			// Needed for extern global_level and leveling logic
#include "level_generate.h"			// Needed for set_map() Function
#include "level_transition.h"		// Needed to transit to Level Transition state
#include "level_overlay.h"			// Needed for Overlays during Welcome, Pause, Reset, Game Over & Game End
#include "easydraw.h"				// Needed for Customised Buttons/Texts
#include "basegame.h"		/* TODO Help State what this is for */
#include <stdio.h>			/* TODO Help State what this is for */
#include <stdlib.h>			/* TODO Help State what this is for */
#include <math.h>			/* TODO Help State what this is for */

extern Config config;
Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER_MAX];
Teleporter teleporters[TELEPORTER_NUMBER];
Flag keyTriggered;

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];										// Array for Waypoints
char* stat[4];																		// Game Stats to be Displayed/Printed

float cellSize, cellAlign, elapsedLock, total_elapsed_time, oneSecondFlip, cameratogglecooldown, cusDelay;

int arrowPosY;
int total_objectives,																// Objectives to Meet
isLocked, activatedCusX, activatedCusY, stunner,									// Stunner/Customer Logic Variables/Flags
face, isAnimating, flip, cameratoggle,												// Drawing/Animation Logic Variables/Flags
game_pause, overlay_function, is_welcome, reset_confirmed, gameover, game_end,		// Pause/Overlays Logic Variables/Flags
clock, times_distracted, duration_lost,												// Stats Variables
time_lost, ignore_penalty,															// Unique Mechanic Variables
play30;																				// SFX Flag

CP_Sound fail = NULL, success = NULL, push = NULL, teleport_sound = NULL, levelBGM = NULL, level30BGM = NULL, gameMusic;

/* GIF */
static float gifElasped;
static const float DISPLAY_DURATION = .5f;
GIF speechSprite;

void base_Init(void) {

	/* Settings */
	CP_Settings_StrokeWeight(0.5f);									// Stroke thickness between Cells

	// for clock settings
	CP_Settings_TextSize((float)config.settings.resolutionHeight * 0.025f);

	/* Initializations */
	cameratoggle = 1;
	cellSize = 0.f;
	face = 0;
	elapsedLock = 0.f;
	isLocked = 0;
	total_elapsed_time = 0.f;
	cameratogglecooldown = 0.f;
	cusDelay = 1.5f;
	isAnimating = 0;
	flip = 0;
	oneSecondFlip = 0;
	play30 = 0;
	game_pause = 0;													// Flag whether game is Paused/Not, Set to 0 for Unpause
	reset_confirmed = 0;											// Flag to Check whether Resetting the map confirmed, 0 for no Reset
	times_distracted = 0;											// Stat for Number of Times Distracted, 0 at Level Load
	duration_lost = 0;												// Stat for Duration Lost getting distracted, 0 at Level Load
	is_welcome = 1;													// Set to 1 To start off Level 1 with a Welcome Message
	game_end = 0;
	overlay_function = 0;											// 1 for Welcome Message, 2 for Reset, 3 for Pause, 4 for Game Over, 5 for Game End
	gameover = 0;													// 1 for Time Limit, 2 for Customer, 3 for Move Limit
	stat[0] = "Time Left: ";										// Text Stat to Print for Time Left
	stat[1] = "Move: ";												// Text Stat to Print for Move Count
	stat[2] = "Times Distracted: ";									// Text Stat to Print for Number of Times Distracted
	stat[3] = "Time Wasted: ";										// Text Stat to print for Time Wasted Getting Distracted
	time_lost = 45;													// default time lost if distracted
	keyTriggered = NO;

	initControls();													// Initialise Controls
	initArrow();													// Initialise Arrow Indicator

	load_spritesheet(&cellSize, cameratoggle);
	cellAlign = (float)((config.settings.resolutionWidth - (int)cellSize * SOKOBAN_GRID_COLS) / 2);
	set_map(grid, customer, path, teleporters);						// Initialise Map
	global_move = 1;												// Initialise move with 1 for rendering purposes

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[0][row][col].player = 0;							//Initialise to 0 for rendering purposes
			if (grid[row][col].player) {
				arrowPosY = row - 1;								// Initialise Arrow Indicator posY to 1 cell above Player
			}
		}
	}


	/* TODO Remove Unnecessary Comments */

	/*Unique mechanics Initialisation*/
	// to force mechanic enabler check card_effect() for flag details
	//UM.flags = 0;
	//UM.flags |= 32;	// uncomment this line to enable teleporter. cast flags before mechanic_flags()
	//UM.flags |= 4;
	//mechanic_flags();												// Needs to be after set_map() | Disables 2 customers/boxes/keys every stage by default | Initialise time_lost and ignore_penalty
	total_objectives = get_objectives(grid);						// Needs to be after set_map() & mechanic_flags() | Get Number of Objectives to meet (Number of Keys)
	// If mechanic_flags is enabled, make sure it is also enabled in RESET

	/* GIF */
	setGIF(&speechSprite, "./Assets/Spritesheet/speech.png", 1, 4, 0, 0, cellSize);
	gifElasped = 0.f;

	/* SFX */
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
	levelBGM = CP_Sound_Load("./Assets/Sound/Level_BGM.wav");					// Level Background Music
	level30BGM = CP_Sound_Load("./Assets/Sound/Level_BGM_30s.wav");				// Level Background Music when 30 seconds left
	fail = CP_Sound_Load("./Assets/Sound/SFX/Fail.wav");						// Fail SFX
	success = CP_Sound_Load("./Assets/Sound/SFX/Success.wav");					// Successs/Level Clear SFX
	push = CP_Sound_Load("./Assets/Sound/SFX/Push.wav");						// Box Pushing SFX
	teleport_sound = CP_Sound_Load("./Assets/Sound/SFX/Teleport.wav");			// Teleporting SFX
	CP_Sound_PlayAdvanced(levelBGM, 0.5, 1, TRUE, CP_SOUND_GROUP_MUSIC);
}

void base_Update(void) {
	int playerRow, playerCol, is_completed = 0;									// Declare & initialise needed
	float current_elapsed_time = CP_System_GetDt();								// Get Time Pass per frame
	if (CP_Input_KeyTriggered(KEY_ANY)) {
		keyTriggered = YES;
	}

	/* When ESC/P Key pressed, pause/unpause the game */
	if (CP_Input_KeyTriggered(KEY_P) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
		if (!game_pause && (overlay_function == 0 || overlay_function == 2))
			overlay_function = 3;												// 3 for Pause Overlay
		else if (game_pause && overlay_function == 3)
			overlay_function = 0;												// 0 to remove any Overlays
		game_pause = !game_pause;												// Toggle Pause State
	}

	/* Read Grid */
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {

			/* Increment is_completed when a box is in a key */
			if (grid[row][col].key && grid[row][col].box)
				is_completed++;

			/* Get position of player */
			if (grid[row][col].player) {
				playerRow = row;
				playerCol = col;
			}
		}
	}

	/* Display Welcome Message at the start of Level 1 */
	if (global_level == 1 && is_welcome == 1) {
		overlay_function = 1;													// 1 for Welcome Message Overlay
		game_pause = 1;															// Enter Pause State
	}
	else {
		is_welcome = 0;															// Other Levels do not have Welcome Message
	}

	if (!game_pause) {
		int temp = 0;
		if (temp = customerLock(grid, customer)) {
			isLocked = 1;
			stunner = temp - 1;
		}

		CP_Settings_NoTint();													// Clear Any Existing Tint
		total_elapsed_time += current_elapsed_time;								// Total time pass is the summation of current time pass each frame
		cameratogglecooldown += current_elapsed_time;
		clock = duration - (int)total_elapsed_time;								// Remaining time displayed
		gifElasped += CP_System_GetDt();

		oneSecondFlip += current_elapsed_time;
		if (oneSecondFlip >= 1.f) {
			flip = !flip;
			oneSecondFlip = 0;
		}

		/* If all Objectives Met/Level Cleared, Move to Level Transition Screen */
		if (is_completed == total_objectives) {
			if (global_level == 10) {											// When Game Ends at Level 10
				reset_level();													// Reset Global_Level back to 1
				config.save.lastLevelPlayed = global_level;						// Set config.save.lastLevelPlayed to 1	
				writeConfig(config);											// Save to config.dat
				overlay_function = 5;											// 5 for Game End Overlay
				game_pause = 1;													// Enter Pause State
			}
			else {
				next_level();													// Increment global_level
				config.save.lastLevelPlayed = global_level;						// Set new level into config.dat
				writeConfig(config);											// Save to config.dat
				CP_Engine_SetNextGameState(Level_Transition_Init, Level_Transition_Update, Level_Transition_Exit);	// Transit to level_transition state
			}
		}

		/* Lose Condition, When Time's Up */
		if (clock <= 0) {
			CP_Sound_PlayAdvanced(fail, 1, 1, FALSE, CP_SOUND_GROUP_SFX);		// Play Game Over SFX
			CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
			CP_Sound_PlayAdvanced(levelBGM, 1, 1, TRUE, CP_SOUND_GROUP_MUSIC);
			overlay_function = 4;												// 4 for Game Over Overlay
			gameover = 1;														// 1 for Time limit Lose Condition Message
			game_pause = 1;														// Enter Pause State
			card_init();														// Resets all card selection/flags
		}

		/* Lose Condition, Move Limit Reached */
		if (((global_move - 1) >= move_limit)) {
			CP_Sound_PlayAdvanced(fail, 1, 1, FALSE, CP_SOUND_GROUP_SFX);		// Play Game Over SFX
			CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
			CP_Sound_PlayAdvanced(levelBGM, 1, 1, TRUE, CP_SOUND_GROUP_MUSIC);
			overlay_function = 4;												// 4 for Game Over Overlay
			gameover = 3;														// 3 for Move Limit Lose Condition Message
			game_pause = 1;														// Enter Paause State
			card_init();														// Resets all card selection/flags
		}

		/* Play less than 30 second BGM */
		if (clock <= 30 && !play30) {
			CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
			play30 = 1;
			CP_Sound_PlayAdvanced(level30BGM, 1, 1, TRUE, CP_SOUND_GROUP_MUSIC);
		}

		/*If player is stunlocked by customer, all inputs should be ignored.*/
		player_status(&isLocked); // UM
		if (isLocked) {
			if (!isAnimating) {
				// Moves the Customer to the player
				int newDir = customerMoveToPlayer(playerRow, playerCol, stunner, grid, customer);
				face = newDir > 0 ? newDir : face;
			}

			/*Check if 3 seconds has passed*/
			if (elapsedLock <= lockTimer) {
				elapsedLock += current_elapsed_time;
				printf("Locked!\n");
			}
			else {
				/*Reset timer and turn customer inactive*/
				elapsedLock = 0;
				isLocked = 0;
				if (ignore_penalty == 0) {		// UM Elusive card. Ignore time penalty once per stage
					duration -= time_lost;
					duration_lost += time_lost;
					times_distracted++;
				}
				else if (ignore_penalty == 1)
					ignore_penalty = 0;		// Disables flag after getting distracted once
				printf("Unlocked.\n");
			}
		}

		/*All inputs go here.*/
		else {
			/*Movement logic*/

			/*Check for input and get the direction of the input*/
			int dir = getDirection();

			/* Set direction that the player is facing */
			switch (dir) {
			case 1:																// FACE UP
				face = 1;
				break;
			case 2:																// FACE LEFT
				face = 2;
				break;
			case 3:																// FACE DOWN
				face = 3;
				break;
			case 4:																// FACE RIGHT
				face = 4;
				break;
			}

			/* If there is Movement */
			int pushBox = 0;
			if (dir > 0) {
				save_move(moves, grid);											// Save previous grid state
				pushBox = getCell(&playerRow, &playerCol, dir, grid, teleporters);
			}

			// Pushing Box around
			if (pushBox == 1) {
				CP_Sound_PlayAdvanced(push, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			}

			// Pushing Box into objective
			if (pushBox == 2) {
				CP_Sound_PlayAdvanced(success, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			}

			/* Undo Move */
			if (CP_Input_KeyTriggered(KEY_U) && global_move > 1) {
				undo_move(moves, grid);											// Undo Move by loading previous grid state
				face = 0;														// Reset Direction
			}

			else if (CP_Input_KeyTriggered(KEY_C) && cameratogglecooldown > 0.5f) {
				cameratogglecooldown = 0.f;
				cameratoggle = (cameratoggle == 1) ? 2 : 1;
				init_spritesheet(&cellSize, cameratoggle);
			}

			else if (CP_Input_KeyTriggered(KEY_R)) {
				overlay_function = 2;											// 2 for Reset Overlay
				game_pause = 1;
			}
		}

		// Customer Logic
		cusDelay += CP_System_GetDt();
		if (cusDelay > 1.5f) {
			cusDelay = 0.f;
			for (int i = 0; i < CUSTOMER_MAX; i++) {
				if (customer[i].isActive && !customer[i].isIdle && !customer[i].isRandom) {
					customerMovement(i, grid, path, customer);
				}

				if (customer[i].isActive && customer[i].isIdle) {
					customerIdle(i, customer);
				}
				else if (customer[i].isActive && customer[i].isRandom) {
					randomCustomerMovement(i, grid, customer);
				}
			}
		}

	} // End of if(!game_pause) Brace

	/* Rendering */
	draw_background(); // clears and draw background art

	/* Draw Controls */
	if (cameratoggle == 1) {
		drawControls();
	}

	if (cameratoggle == 2)
		world_camera(cellSize, cellAlign, playerRow, playerCol, face, cameratoggle);

	/* Map Rendering */
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize * (float)col + cellAlign;
			float cellY = cellSize * (float)row + cellSize * 0.85f;

			if (!currCell.boarder) draw_floor(cellX, cellY, cellSize);

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.shelf || moves[global_move - 1][row][col].player ||
				currCell.mecha || teleporter[0] == 1 && currCell.tele != 0) {

				if (currCell.boarder)
					draw_boarder(cellX, cellY, cellSize, row, col);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX, cellY, cellSize);

				else if (currCell.key)
					draw_key(cellX, cellY, cellSize);

				else if (currCell.mecha)
					draw_mecha(cellX, cellY, cellSize);

				else if (currCell.box)
					draw_box(cellX, cellY, cellSize);

				else if (currCell.shelf) {
					if (row % 2 || col % 2)
						draw_shelf(cellX, cellY, cellSize, 1);
					else
						draw_shelf(cellX, cellY, cellSize, 2);
				}

				else if (teleporter[0] == 1 && currCell.tele != 0) {
					if ((row == teleporters[currCell.tele - 1].posY) && col == teleporters[currCell.tele - 1].posX) {
						draw_teleporter(cellX, cellY, cellSize);
					}
				}
			}
		}
	}

	for (int i = 0; i < CUSTOMER_MAX; i++) {
		if (grid[customer[i].cusRow][customer[i].cusCol].customer)
			draw_customer(cellSize, cellAlign, customer[i].cusRow, customer[i].cusCol, customer[i].direction, i, cameratoggle);
	}

	isAnimating = draw_player(cellSize, cellAlign, playerRow, playerCol, face, cameratoggle);

	/* Draw Arrow Indicator if no key triggered*/
	if (!keyTriggered) {
		if (!(CP_System_GetFrameCount() % 8)) {
			if (arrowPosY > playerRow - 2) {
				arrowPosY--;
			}
			else if (arrowPosY < playerRow - 1) {
				arrowPosY++;
			}
		}
		drawArrow(cellSize, cellAlign, arrowPosY, playerCol);
	}

	if (isLocked && !isAnimating) {
		speechSprite.gifDimensions = cellSize;
		if (flip) {
			if (face == 2 || face == 3) {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(playerCol + 1) + cellAlign, cellSize * (float)(playerRow - 1) + cellSize * 0.85f);
				drawGIF(&speechSprite, &gifElasped, 0.1f, NO, NO);
			}
			else {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(playerCol - 1) + cellAlign, cellSize * (float)(playerRow - 1) + cellSize * 0.85f);
				drawGIF(&speechSprite, &gifElasped, 0.1f, YES, NO);
			}
		}
		if (!flip) {
			if (customer[stunner].direction == 2 || customer[stunner].direction == 3) {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(customer[stunner].cusCol + 1) + cellAlign, cellSize * (float)(customer[stunner].cusRow - 1) + cellSize * 0.85f);
				drawGIF(&speechSprite, &gifElasped, 0.1f, NO, NO);
			}
			else {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(customer[stunner].cusCol - 1) + cellAlign, cellSize * (float)(customer[stunner].cusRow - 1) + cellSize * 0.85f);
				drawGIF(&speechSprite, &gifElasped, 0.1f, YES, NO);
			}
		}
	}


	if (cameratoggle == 2)
		revert_world_camera();

	/* When in Pause State, Render Different Overlays based on 'overlay_function' */
	if (game_pause) {
		/* Welcome Message at Level 01 */
		if (global_level == 1 && overlay_function == 1) {					// If Level is 1 & overlay_function is 1
			overlay_welcome();												// Rednders Welcome Message
			is_welcome = welcome_done(game_pause);							// Remove is_welcome flag
			overlay_function = welcome_done(game_pause);					// Remove Overlays
			game_pause = welcome_done(game_pause);							// Unpause game
		}
		/* Resetting Map Overlay */
		else if (overlay_function == 2) {									// Else If overlay_function is 2
			overlay_reset();												// Renders Reset Confirmation Overlay
			reset_confirmed = reset_check(reset_confirmed);					// Check Whether 'YES' or 'NO' was Clicked

			/* If 'YES' was Clicked */
			if (reset_confirmed == 1) {
				CP_Engine_SetNextGameStateForced(base_Init, base_Update, base_Exit);	// Force load basegame.c/basegame state
			}

			/* If 'NO' was Clicked */
			else if (reset_confirmed == 2) {
				reset_confirmed = 0;										// Set reset_confirmed to 0 so that will stop rendering Reset Overlay
				overlay_function = 0;										// Remove overlays
				game_pause = 0;												// Set game_pause to 0 to resume game
			}
		}

		/* Pause Overlay */
		else if (overlay_function == 3) {									// Else If  ovarlay_function is 3
			overlay_pause();												// Renders Pause Overlay
			game_pause = unpause(game_pause);								// game_pause will trigger unpause function to either return to game or Main Menu
		}

		/* Game Over Overlay */
		else if (overlay_function == 4) {									// Else if overlay_function is 4
			overlay_game_over(gameover - 1);								// Renders Game Over Overlay
			game_pause = game_over(game_pause);								// game_pause will trigger to return to Main Menu
		}

		else if (overlay_function == 5) {									// Else of overlay_function is 5
			overlay_end_game();												// Renders Game End Overlay
			end_game();														// Returns to Main Menu when Button is Clicked
		}

	}

	/* Show Stats */
	show_stats(cellSize, stat[0], clock, cameratoggle, 1.f);				// Show Timer
	show_stats(cellSize, stat[1], global_move - 1, cameratoggle, 2.f);		// Show Move Count
	show_stats(cellSize, stat[2], times_distracted, cameratoggle, 3.f);		// Show Number of Times Distracted Count
	show_stats(cellSize, stat[3], duration_lost, cameratoggle, 4.f);		// Show Duration Lost
}

void base_Exit(void) {

	/* Free Assets */
	CP_Image_Free(speechSprite.spritesheet);
	CP_Sound_Free(&fail);
	CP_Sound_Free(&push);
	CP_Sound_Free(&success);
	CP_Sound_Free(&teleport_sound);
	CP_Sound_Free(&levelBGM);
	CP_Sound_Free(&level30BGM);
	CP_Sound_PlayAdvanced(gameMusic, 1, 1, TRUE, CP_SOUND_GROUP_MUSIC);
	free_sprite();
	CP_Settings_StrokeWeight(3.0f);
	freeControls();
	freeArrow();
	//CP_Image_Free(&speechSprite);
}