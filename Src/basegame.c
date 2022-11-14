#include "cprocessing.h"			// Needed for C Processing Functions
#include "structs.h"				// Needed for Game Asset Structs
#include "customer.h"
#include "utils.h"
#include "defines.h"				// Needed for Defined Values
#include "settings.h"
#include "spritesheet.h"
#include "mechanics.h"
#include "mainmenu.h"
#include "movement.h"				// Needed for saveMove(), undoMove() & resetMap() functions
#include "level_logic.h"			// Needed for extern global_level and leveling logic
#include "level_generate.h"			// Needed to Generate Game Map
#include "level_transition.h"		// Needed to transit to Level Transition state
#include "level_overlay.h"			// Needed for Overlays during Welcome, Pause, Reset or Game Over
#include "easydraw.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern Config config;
Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER_MAX];
Teleporter teleporters[TELEPORTER_NUMBER];

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
char* stat[4];

float cellSize, cellAlign, elapsedLock, totalElapsedTime, oneSecondFlip, cameratogglecooldown, cusDelay;

int total_objectives,											// Objectives to Meet
isLocked, activatedCusX, activatedCusY, stunner,				// Stunner/Customer Logic Variables/Flags
face, isAnimating, flip, cameratoggle,							// Drawing/Animation Logic Variables/Flags
game_pause, overlay_function, is_welcome, reset_confirmed,		// Pause/Overlays Logic Variables/Flags
clock, times_distracted, duration_lost,							// Stats Variables
time_lost, ignore_penalty;										// Unique Mechanic Variables

CP_Sound fail = NULL, success = NULL, push = NULL, teleport = NULL, levelBGM = NULL, gameMusic;

/* GIF */
static float gifElasped;
static const float DISPLAY_DURATION = .5f;
GIF speechSprite;

void base_Init(void) {

	/* Settings */
	CP_Settings_StrokeWeight(0.5f);

	// for clock settings
	CP_Settings_TextSize((float)config.settings.resolutionHeight * 0.025f);

	/* Initializations */
	cameratoggle = 1;
	cellSize = 0.f;
	face = 0;
	elapsedLock = 0.f;
	isLocked = 0;
	totalElapsedTime = 0.f;
	cameratogglecooldown = 0.f;
	cusDelay = 1.5f;
	isAnimating = 0;
	flip = 0;
	oneSecondFlip = 0;
	game_pause = 0;													// Flag whether game is Paused/Not, Set to 0 for Unpause
	reset_confirmed = 0;											// Flag to Check whether Resetting the map confirmed, 0 for no Reset
	times_distracted = 0;											// Stat for Number of Times Distracted, 0 at Level Load
	duration_lost = 0;												// Stat for Duration Lost getting distracted, 0 at Level Load
	is_welcome = 1;													// Set to 1 To start off Level 1 with a Welcome Message
	overlay_function = 0;											// 1 for Welcome Message, 2 for Reset, 3 for Pause, 4 for Game Over
	stat[0] = "Time Left: ";										// Text Stat to Print for Time Left
	stat[1] = "Move: ";												// Text Stat to Print for Move Count
	stat[2] = "Times Distracted: ";									// Text Stat to Print for Number of Times Distracted
	stat[3] = "Time Wasted: ";										// Text Stat to print for Time Wasted Getting Distracted

	load_spritesheet(&cellSize, cameratoggle);
	setMap(grid, customer, path, teleporters);						// Initialise Map
	total_objectives = getObjective(grid);							// Counts number of key objective to meet
	global_move = 1;												// Initialise move with 1 for rendering purposes*
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0, m = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[0][row][col].player = 0;							//Initialise to 0 for rendering purposes
		}
	}
	/*Unique mechanics Initialisation*/
	// to force mechanic enabler check card_effect() for flag details
	// UM.flags |= 32;	// uncomment this line to enable teleporter. cast flags before mechanic_flags()
	mechanic_flags(); // Needs to be after setMap() | Disables 2 customers/boxes/keys every stage by default | Initialise time_lost and ignore_penalty
	
	/*GIF*/
	setGIF(&speechSprite, "./Assets/Spritesheet/speech.png", 1, 4, 0, 0, cellSize);
	gifElasped = 0.f;

	/* SFX */
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
	levelBGM = CP_Sound_Load("./Assets/Sound/Level_BGM.wav");
	fail = CP_Sound_Load("./Assets/Sound/SFX/Fail.wav");
	success = CP_Sound_Load("./Assets/Sound/SFX/Success.wav");
	push = CP_Sound_Load("./Assets/Sound/SFX/Push.wav");
	CP_Sound_PlayAdvanced(levelBGM, 0.5, 1, TRUE, CP_SOUND_GROUP_MUSIC);
}

void base_Update(void) {
	int playerRow, playerCol, isCompleted = 0, set_teleporter_row = 0, set_teleporter_col = 0;
	float currentElapsedTime = CP_System_GetDt();

	/* When ESC/P Key pressed, pause/unpause the game */
	if (CP_Input_KeyTriggered(KEY_P) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
		if (!game_pause && (overlay_function == 0 || overlay_function == 2))
			overlay_function = 3;												// 3 for Pause Overlay
		else if (game_pause && overlay_function == 3)
			overlay_function = 0;												// 0 to remove any Overlays
		game_pause = !game_pause;												// Toggle Pause State
	}

	/*Read grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			/*Check if all objectives has been reached*/
			if (grid[row][col].key && grid[row][col].box)
				isCompleted++;

			/*Get position of player*/
			if (grid[row][col].player) {
				playerRow = row;
				playerCol = col;
			}
		}
	}

	/* Display Welcome Message at the start of Level 1 */
	if (global_level == 1 && is_welcome == 1) {
		overlay_function = 1;													// 1 for Welcome Message Overlay
		game_pause = 1;
	}

	if (!game_pause) {
		int temp = 0;
		if (temp = customerLock(grid, customer)) {
			isLocked = 1;
			stunner = temp - 1;
		}

		/*Clear Tint*/
		CP_Settings_NoTint();
		totalElapsedTime += currentElapsedTime;
		cameratogglecooldown += currentElapsedTime;
		clock = duration - (int)totalElapsedTime;
		gifElasped += CP_System_GetDt();

		oneSecondFlip += currentElapsedTime;
		if (oneSecondFlip >= 1.f) {
			flip = !flip;
			oneSecondFlip = 0;
		}

		/* If all Objectives Met/Level Cleared, Move to Level Transition Screen */
		if (isCompleted == total_objectives) {
			next_level();
			config.save.lastLevelPlayed = global_level;
			writeConfig(config);
			CP_Engine_SetNextGameState(Level_Transition_Init, Level_Transition_Update, Level_Transition_Exit);
		}

		/* Lose Condition, When Time's Up */
		if (clock <= 0) {
			CP_Sound_PlayAdvanced(fail, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			overlay_function = 4;												// 4 for Game Over Overlay
			game_pause = 1;
			card_init();	// resets all card selection/flags
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
				elapsedLock += currentElapsedTime;
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
				else if(ignore_penalty == 1) 
					ignore_penalty = 0;		// Disables flag after getting distracted once
				printf("Unlocked.\n");
			}
		}

		/*All inputs go here.*/
		else {
			/*Movement logic*/

			/*Check for input and get the direction of the input*/
			int dir = getDirection();

			/*Set direction that the player is facing.*/
			switch (dir) {
			case 1: // up
				face = 1;
				break;
			case 2: // left
				face = 2;
				break;
			case 3: // down
				face = 3;
				break;
			case 4: // right
				face = 4;
				break;
			}

			/*If there is movement.*/
			int pushBox = 0;
			if (dir > 0) {
				saveMove(moves, grid);
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
				undoMove(moves, grid);
				face = 0;
			}

			else if (CP_Input_KeyTriggered(KEY_C) && cameratogglecooldown > 0.5f) {
				cameratogglecooldown = 0.f;
				cameratoggle = (cameratoggle == 1) ? 2 : 1;
				init_spritesheet(&cellSize, cameratoggle);
			}

			else if (CP_Input_KeyTriggered(KEY_R)) {
				overlay_function = 2;
				game_pause = 1;
			}
		}

		// Customer Logic
		cusDelay += CP_System_GetDt();
		if (cusDelay > 1.5f) {
			cusDelay = 0.f;
			for (int i = 0; i < CUSTOMER_MAX; i++) {
				if (customer[i].isActive && !customer[i].isIdle && !customer[i].isRandom) {
					customerMovement(i ,grid, path, customer);
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

	if (cameratoggle == 2)
		world_camera(cellSize, playerRow, playerCol, face, cameratoggle);

	/* Map Rendering */
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize * (float)col;
			float cellY = cellSize * (float)row;

			draw_floor(cellX, cellY, cellSize);

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.shelf || moves[global_move - 1][row][col].player ||
				currCell.mecha || teleporter[0] == 1 && currCell.tele != 0) {
				if (currCell.boarder)
					draw_boarder(cellX, cellY, cellSize);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX, cellY, cellSize);

				else if (currCell.key)
					draw_key(cellX, cellY, cellSize);

				else if (currCell.mecha)
					draw_boarder(cellX, cellY, cellSize); // draw_mecha();

				else if (currCell.box)
					draw_box(cellX, cellY, cellSize);

				else if (currCell.shelf)
					draw_boarder(cellX, cellY, cellSize);

				else if (teleporter[0] == 1 && currCell.tele != 0) {
						if ((row == teleporters[currCell.tele-1].posY) && col == teleporters[currCell.tele-1].posX) {
							draw_boarder(cellX, cellY, cellSize); // draw_tele();
						}
				}
			}
		}
	}

	for (int i = 0; i < CUSTOMER_MAX; i++)
		draw_customer(cellSize, customer[i].cusRow, customer[i].cusCol, customer[i].direction, i, cameratoggle);

	isAnimating = draw_player(cellSize, playerRow, playerCol, face, cameratoggle);

	if (isLocked && !isAnimating) {
		if (flip) {
			if (face == 2 || face == 3) {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(playerCol + 1), cellSize * (float)(playerRow - 1));
				drawGIF(&speechSprite, &gifElasped, 0.1, NO, NO);
			}
			else {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(playerCol - 1), cellSize * (float)(playerRow - 1));
				drawGIF(&speechSprite, &gifElasped, 0.1, YES, NO);
			}
		}
		if (!flip) {
			if (customer[stunner].direction == 2 || customer[stunner].direction == 3) {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(customer[stunner].cusCol + 1), cellSize * (float)(customer[stunner].cusRow - 1));
				drawGIF(&speechSprite, &gifElasped, 0.1, NO, NO);
			}
			else {
				speechSprite.position = CP_Vector_Set(cellSize * (float)(customer[stunner].cusCol - 1), cellSize * (float)(customer[stunner].cusRow - 1));
				drawGIF(&speechSprite, &gifElasped, 0.1, YES, NO);
			}
		}
	}

	
	if (cameratoggle == 2)
		revert_world_camera();

	if (game_pause) {
		/* Welcome Message at Level 01 */
		if (global_level == 1 && overlay_function == 1) {					// If Level is 1, is_welcome flag triggered & Not yet Game Over
			overlay_welcome();												// Rednders Welcome Message
			is_welcome = welcome_done(game_pause);							// Remove is_welcome flag
			overlay_function = welcome_done(game_pause);
			game_pause = welcome_done(game_pause);							// Unpause game
		}
		/* Resetting Map Overlay */
		else if (overlay_function == 2) {			// Else If reset_triggered flag is trigggered, There is still time & Not yet Game Over
			overlay_reset();												// Renders Reset Confirmation Overlay
			reset_confirmed = reset_check(reset_confirmed);					// Check Whether 'YES' or 'NO' was Clicked

			/* If 'YES' was Clicked */
			if (reset_confirmed == 1) {
				resetMap(moves, grid, customer, path, teleporters);			// Resets grid to the initial values based on the CSV file
				totalElapsedTime = 0;										// Reset Timer
				face = 0;													// Reset Player Direcction
				reset_confirmed = 0;										// Set reset_confirmed to 0 so that will stop rendering Reset Overlay
				times_distracted = 0;										// Reset the display of number of timesdistracted
				duration_lost = 0;											// Reset the display of duration lost due to distractions
				overlay_function = 0;
				game_pause = 0;												// Set game_pause to 0 to resume game
			}

			/* If 'NO' was Clicked */
			else if (reset_confirmed == 2) {
				reset_confirmed = 0;										// Set reset_confirmed to 0 so that will stop rendering Reset Overlay
				overlay_function = 0;
				game_pause = 0;												// Set game_pause to 0 to resume game
			}
		}

		/* Pause Overlay */
		else if (overlay_function == 3) {									// Else If 
			overlay_pause();												// Renders Pause Overlay
			game_pause = unpause(game_pause);								// game_pause will trigger unpause function to either return to game or Main Menu
		}

		/* Game Over Overlay */
		else if (overlay_function == 4) {
			overlay_game_over();											// Renders Game Over Overlay
			game_pause = game_over(game_pause);								// game_pause will trigger to return to Main Menu
		}

	}

	/* Show Stats */
	show_stats(cellSize, stat[0], clock, cameratoggle, 1.f);				// Show Timer
	show_stats(cellSize, stat[1], global_move, cameratoggle, 2.f);			// Show Move Count
	show_stats(cellSize, stat[2], times_distracted, cameratoggle, 3.f);		// Show Number of Times Distracted Count
	show_stats(cellSize, stat[3], duration_lost, cameratoggle, 4.f);		// Show Duration Lost

}

void base_Exit(void) {
	CP_Image_Free(speechSprite.spritesheet);
	CP_Sound_Free(&fail);
	CP_Sound_Free(&push);
	CP_Sound_Free(&success);
	CP_Sound_Free(&levelBGM);
	CP_Sound_PlayAdvanced(gameMusic, 1, 1, TRUE, CP_SOUND_GROUP_MUSIC);
	free_sprite();
	CP_Settings_StrokeWeight(3.0f);
	//CP_Image_Free(&speechSprite);
}