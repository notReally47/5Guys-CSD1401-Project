#include "cprocessing.h"
#include "structs.h"
#include "utils.h"
#include "defines.h"
#include "customer.h"
#include "movement.h"
#include "generateLevel.h"
#include "spritesheet.h"
#include <stdio.h>

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER];

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellSize, cellAlign, sec, elapsedLock;

int totalObjs, isLocked, activatedCusX, activatedCusY, move, face, time, level;

void base_Init(void) {
	// initialisation  || IN BASEGAME TEMPORARILY
	CP_System_SetWindowSize(CP_System_GetDisplayWidth() / 2, CP_System_GetDisplayHeight() / 2); // >>1
	//if (CP_System_GetWindowWidth() == CP_System_GetDisplayWidth())
		//CP_System_Fullscreen();  // set fullscreen if max
	CP_System_SetWindowTitle("SevenTwee");


	move = 0; //Initialise move with 0
	setMap(grid, customer); //Initialise map
	totalObjs = getObjective(grid); //Counts number of key objective to meet

	/*Settings*/
	CP_Settings_StrokeWeight(0.5f);
	CP_Settings_RectMode(CP_POSITION_CORNER);

	/*Initializations*/
	cellSize = (float)(CP_System_GetWindowHeight() / SOKOBAN_GRID_ROWS);
	cellAlign = (float)((CP_System_GetWindowWidth() - (int)cellSize * SOKOBAN_GRID_COLS) / 2);
	face = 0;
	load_spritesheet(cellSize);
}

void base_Update(void) {
	/*Check for input and get the direction of the input*/
	int dir = getDirection();

	int playerPosX, playerPosY, isCompleted = 0;

	/*Read grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			/*Check if all objectives has been reached*/
			if (grid[col][row].key && grid[col][row].box)
				isCompleted++;

			/*Get position of player*/
			if (grid[row][col].player) {
				playerPosX = row;
				playerPosY = col;
			}
			if (customerLock(grid, customer)) {
				isLocked = 1;
			}

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
		}
	}

	/*If all objectives reached, do something here*/
	if (isCompleted == totalObjs) {
		// return to main menu probably
		// CP_Engine_SetNextGameState(Main_Menu_Init,Main_Menu_Update,Main_Menu_Exit);
	}

	/*Game logic*/
	/*If player is stunlocked by customer*/
	if (isLocked) {
		/*Check if 3 seconds has passed*/
		if (elapsedLock < 3) {
			elapsedLock += CP_System_GetDt();
			printf("LOCKED\n");
		}
		else {
			/*Reset timer and turn customer inactive*/
			elapsedLock = 0;
			isLocked = 0;
			printf("UNLOCKED\n");
		}
	}

	else {
		if (dir > 0) {
			move = moveCount(move, moves, grid);
			getCell(playerPosX, playerPosY, dir, grid);
		}
		if (CP_Input_KeyTriggered(KEY_U)) {
			move = undoMove(move, moves, grid);
		}
		//else if (CP_Input_KeyTriggered(KEY_R)) {
		//	move = resetMap(move, moves, grid);
		//}
	}

	//customerMovement(grid, path, customer);

	for (int i = 0; i < CUSTOMER; i++) {
		if (customer[i].isIdle) {
			customerIdle(i, customer);
		}
		else if (customer[i].isRandom) {
			randomCustomerMovement(grid, customer);
		}
	}

	if (CP_Input_KeyDown(KEY_U)) {
		move = undoMove(move, moves, grid); //Undo a move and set grid to the previous move based on 'moves' array
	}
	else if (CP_Input_KeyTriggered(KEY_R)) {
		move = resetMap(move, moves, grid, customer); //Resets grid to the initial values based on the CSV file
	}

	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize * (float)col + cellAlign;
			float cellY = cellSize * (float)row;


			draw_floor(cellX, cellY, cellSize);

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.shelf) {
				if (currCell.boarder)
					draw_boarder(cellX, cellY, cellSize);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX, cellY, cellSize);

				else if (currCell.key) {
					draw_key(cellX, cellY, cellSize);
					if (currCell.player)
						goto player;
				}

				else if (currCell.player) player:
				draw_player(cellX, cellY, face);

				else if (currCell.box)
					draw_box(cellX, cellY, cellSize);
				else if (currCell.shelf)
					draw_boarder(cellX, cellY, cellSize);
			}

			if (currCell.customer) {
				for (int i = 0; i < CUSTOMER; i++) {

					if (row == customer[i].posX && col == customer[i].posY) {
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
