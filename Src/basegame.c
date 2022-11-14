#include "cprocessing.h"
#include "structs.h"
#include "customer.h"
#include "level_generate.h"
#include "movement.h"
#include "utils.h"
#include "defines.h"
#include "settings.h"
#include "spritesheet.h"
#include "level_logic.h"
#include "level_transition.h"
#include "mechanics.h"
#include "mainmenu.h"
#include "level_overlay.h"
#include "options_draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern Config config;

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER_MAX];

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
char* stat[3];

float cellSize, cellAlign, sec, elapsedLock, totalElapsedTime, oneSecondFlip, cameratogglecooldown;

int totalObjs, isLocked, activatedCusX, activatedCusY, face, game_pause, clock, stunner, isAnimating, flip, reset_triggered, reset_confirmed, cameratoggle, times_distracted, is_game_over, is_welcome;

CP_Sound fail = NULL, success = NULL, push = NULL, teleport = NULL;

/*Gif*/
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
	game_pause = 0;
	is_game_over = 0;
	isAnimating = 0;
	flip = 0;
	oneSecondFlip = 0;
	reset_triggered = 0;
	reset_confirmed = 0;
	times_distracted = 0;
	is_welcome = 1;
	stat[0] = "Time Left: ";
	stat[1] = "Move: ";
	stat[2] = "Times Distracted: ";

	load_spritesheet(&cellSize, cameratoggle);
	setMap(grid, customer, path);				// Initialise Map
	totalObjs = getObjective(grid);				// Counts number of key objective to meet
	global_move = 1;							// Initialise move with 1 for rendering purposes*
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0, m = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[0][row][col].player = 0; //Initialise to 0 for rendering purposes
		}
	}

	/*GIF*/
	setGIF(&speechSprite, "./Assets/Spritesheet/speech.png", 1, 4, 0, 0, cellSize);
	gifElasped = 0.f;

	/* SFX */
	// Set all audio in SFX group in accordance to the audio settings
	fail = CP_Sound_Load("./Assets/Sound/SFX/Fail.wav");
	success = CP_Sound_Load("./Assets/Sound/SFX/Success.wav");
	push = CP_Sound_Load("./Assets/Sound/SFX/Push.wav");
	//teleport = CP_Sound_Load();
	card_init();
	//teleport_UM();

	
}

void base_Update(void) {
	int playerRow, playerCol, isCompleted = 0, set_teleporter_row = 0, set_teleporter_col = 0;
	float currentElapsedTime = CP_System_GetDt();
	//teleport_UM();

	if (CP_Input_KeyTriggered(KEY_P) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
		game_pause = !game_pause;
		reset_triggered = 0;
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
	if (global_level == 1 && is_welcome) {
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
		if (isCompleted == totalObjs) {
			next_level();
			config.save.lastLevelPlayed = global_level;
			writeConfig(config);
			CP_Engine_SetNextGameState(Level_Transition_Init, Level_Transition_Update, Level_Transition_Exit);
		}

		/* Lose Condition */
		if (clock == 0 || times_distracted > 3) {
			CP_Sound_PlayAdvanced(fail, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			is_game_over = 1;
			game_pause = 1;
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
				times_distracted++;
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
				pushBox = getCell(&playerRow, &playerCol, dir, grid);
			}

			if (pushBox == 1) {
				CP_Sound_PlayAdvanced(push, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			}

			if (pushBox == 2) {
				CP_Sound_PlayAdvanced(success, 1, 1, FALSE, CP_SOUND_GROUP_SFX);
			}

			/*Undo move.*/
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
				reset_triggered = 1;
				game_pause = 1;
			}
		}

		customerMovement(grid, path, customer);

		for (int i = 0; i < CUSTOMER_MAX; i++) {
			if (customer[i].isIdle) {
				customerIdle(i, customer);
			}
			else if (customer[i].isRandom) {
				randomCustomerMovement(grid, customer);
			}
		}
	}

	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

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
					for (int i = 1; i < 9; i++) {
						set_teleporter_row = i * 2 - 1;
						set_teleporter_col = i + i;
						if (row == teleporter[set_teleporter_row] && col == teleporter[set_teleporter_col])
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


	if (game_pause) {

		/* Welcome Message at Level 01 */
		if (global_level == 1 && is_welcome && !is_game_over) {
			overlay_welcome();									// Rednders Welcome Message
			is_welcome = welcome_done(game_pause);				// Remove is_welcome flag
			game_pause = welcome_done(game_pause);				// Unpause game
		}

		/* Resetting Map Overlay */
		else if (reset_triggered && clock > 0) {
			overlay_reset();									// Renders Reset Confirmation Overlay
			reset_confirmed = reset_check(reset_confirmed);		// Check Whether 'YES' or 'NO' was Clicked

			/* If 'YES' was Clicked */
			if (reset_confirmed == 1) {
				resetMap(moves, grid, customer, path);			// Resets grid to the initial values based on the CSV file
				totalElapsedTime = 0;							// Reset Timer
				face = 0;										// Reset Player Direcction
				reset_confirmed = 0;							// Set reset_confirmed to 0 so that will stop rendering Reset Overlay
				reset_triggered = 0;							// Set reset_triggered to 0 so that will stop rendering Reset Overlay
				game_pause = 0;									// Set game_pause to 0 to resume game
			}

			/* If 'NO' was Clicked */
			else if (reset_confirmed == 2) {
				reset_confirmed = 0;							// Set reset_confirmed to 0 so that will stop rendering Reset Overlay
				reset_triggered = 0;							// Set reset_triggered to 0 so that will stop rendering Reset Overlay
				game_pause = 0;									// Set game_pause to 0 to resume game
			}
		}

		/* Pause Overlay */
		else if (!reset_triggered && clock > 0 && !is_game_over) {
			overlay_pause();									// Renders Pause Overlay
			game_pause = unpause(game_pause);					// game_pause will trigger unpause function to either return to game or Main Menu
		}

		/* Game Over Overlay */
		else if (!reset_triggered && is_game_over) {
			overlay_game_over();								// Renders Game Over Overlay
			game_pause = game_over(game_pause);					// game_pause will trigger to return to Main Menu
		}
	}

	/* Show Timer */
	show_stats((float)config.settings.resolutionHeight * 0.05f, cellSize * 55.f, cellSize, stat[0], clock);

	/* Show Move Count */
	show_stats((float)config.settings.resolutionHeight * 0.05f, cellSize * 55.f, cellSize * 3.f, stat[1], (global_move - 1));

	/* Show Distracted Count */
	show_stats((float)config.settings.resolutionHeight * 0.05f, cellSize * 55.f, cellSize * 5.f, stat[2], times_distracted);

}

void base_Exit(void) {
	CP_Image_Free(speechSprite.spritesheet);
	CP_Sound_Free(&fail);
	CP_Sound_Free(&push);
	CP_Sound_Free(&success);
	free_sprite();
	CP_Settings_StrokeWeight(3.0f);
	//CP_Image_Free(&speechSprite);
}