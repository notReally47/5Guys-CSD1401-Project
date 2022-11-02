#include "cprocessing.h"
#include "structs.h"
#include "customer.h"
#include "generateLevel.h"
#include "movement.h"
#include "utils.h"
#include "defines.h"
#include "settings.h"
#include "spritesheet.h"
#include "levellogic.h"
#include "levelTransition.h"
#include <stdio.h>

extern Config config;

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Move moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER];

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellSize, cellAlign, sec, elapsedLock;

int totalObjs, isLocked, activatedCusX, activatedCusY, face, time;

void base_Init(void) {
	
	move = 1; //Initialise move with 1 for rendering purposes*
	setMap(grid, customer); //Initialise map
	totalObjs = getObjective(grid); //Counts number of key objective to meet

	/*Settings*/
	CP_System_SetWindowTitle("SevenTwee");
	// already declared in splash_screen. used for main.c -> basegame.c
	CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_StrokeWeight(0.5f);

	/*Initializations*/
	cellSize = (float)(CP_System_GetWindowHeight()/SOKOBAN_GRID_ROWS);
	cellAlign = (float)((CP_System_GetWindowWidth()-(int)cellSize*SOKOBAN_GRID_COLS)/2);
	face = 0;
	elapsedLock = 0;
	isLocked = 0;
	load_spritesheet(cellSize);
}

void base_Update(void) {
	int playerPosX, playerPosY, isCompleted = 0;
	

	/*Read grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			/*Check if all objectives has been reached*/
			if (grid[row][col].key && grid[row][col].box)
				isCompleted++;

			/*Get position of player*/
			if (grid[row][col].player) {
				playerPosX = row;
				playerPosY = col;
			}

			if (customerLock(grid, customer)) {
				isLocked = 1;
			}
		}
	}
	
	/*If all objectives reached, do something here*/
	if (isCompleted == totalObjs) {
		next_level();
		CP_Engine_SetNextGameState(Level_Transition_Init, Level_Transition_Update, Level_Transition_Exit); // load transition state
	}

	/*If player is stunlocked by customer, all inputs should be ignored.*/
	if (isLocked) {
		/*Check if 3 seconds has passed*/
		if (elapsedLock <= 3) {
			elapsedLock = elapsedLock + CP_System_GetDt();
			printf("Locked!\n");
		}
		else {
			/*Reset timer and turn customer inactive*/
			elapsedLock = 0;
			isLocked = 0;
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
		if (dir > 0) {
			saveMove(moves, grid);
			getCell(playerPosX, playerPosY, dir, grid);
		}

		/*Undo move.*/
		if (CP_Input_KeyTriggered(KEY_U) && move > 1) {
			undoMove(moves, grid);
			face = 0;
		}

		else if (CP_Input_KeyTriggered(KEY_R)) {
			resetMap(moves, grid, customer); //Resets grid to the initial values based on the CSV file
			face = 0;
		}
	}

	//TODO
	//customerMovement(grid, path, customer);

	for (int i = 0; i < CUSTOMER; i++) {
		if (customer[i].isIdle) {
			customerIdle(i, customer);
		}
		else if (customer[i].isRandom) {
			randomCustomerMovement(grid, customer);
		}
	}

	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

	/*Rendering grid*/
	CP_Graphics_ClearBackground(WHITE);
	
	// experimental
	// world_camera(cellSize,face,dir); // requires dir to be declared outside else loop
	
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize*(float)col+cellAlign;
			float cellY = cellSize*(float)row;
			float cellx = cellSize*(float)playerPosY+cellAlign;
			float celly = cellSize*(float)playerPosX;
			
			draw_floor(cellX, cellY, cellSize);

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.shelf || moves[move-1][row][col].player) {
				if (currCell.boarder)
					draw_boarder(cellX, cellY, cellSize);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX, cellY, cellSize);

				else if (currCell.key) 
					draw_key(cellX, cellY, cellSize);

				if ((currCell.player && (face != 1 && face != 2)) || moves[move - 1][row][col].player && (face != 3 && face != 4 && face != 0)) player:
					draw_player(cellx, celly, face);

				else if (currCell.box)
					draw_box(cellX, cellY, cellSize);
				else if (currCell.shelf)
					draw_boarder(cellX, cellY, cellSize);
			}

			if (currCell.customer) {
				for (int i = 0; i < CUSTOMER; i++) {

					if (row == customer[i].cusRow && col == customer[i].cusCol) {
						switch (customer[i].direction) {
						case SOKOBAN_UP:
							CP_Settings_Fill(NEON_PINK);
							break;
						case SOKOBAN_LEFT:
							CP_Settings_Fill(CARNATION);
							break;
						case SOKOBAN_DOWN:
							CP_Settings_Fill(SALMON);
							break;
						case SOKOBAN_RIGHT:
							CP_Settings_Fill(COTTON);
							break;
						default:
							break;
						}
						CP_Graphics_DrawRect(cellX, cellY, cellSize, cellSize);
					}
				}
			}
		}
	}
}

void base_Exit(void) {
	free_sprite();
}
