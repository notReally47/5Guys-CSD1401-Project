#include "cprocessing.h"
#include "structs.h"
#include "utils.h"
#include "defines.h"
#include "spritesheet.h"


Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellSize,cellAlign;
int totalObjs, face;

void base_Init(void) {
	// initialisation  || IN BASEGAME TEMPORARILY
	CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight()); // >>1
	//if (CP_System_GetWindowWidth() == CP_System_GetDisplayWidth())
		//CP_System_Fullscreen();  // set fullscreen if max
	CP_System_SetWindowTitle("SevenTwee");


	/*Create empty grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			grid[row][col].boarder = 0;
			grid[row][col].key = 0;
			grid[row][col].player = 0;
			grid[row][col].box = 0;
		}
	}
	/*Set grid characteristics here*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		grid[row][0].boarder = 1;
		grid[row][SOKOBAN_GRID_COLS - 1].boarder = 1;
	}

	for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
		grid[0][col].boarder = 1;
		grid[SOKOBAN_GRID_ROWS - 1][col].boarder = 1;
	}

	grid[1][1].player = 1;

	grid[5][5].box = 1;
	grid[10][10].box = 1;

	grid[16][12].key = 1;
	grid[3][15].key = 1;

	totalObjs = 2;

	/*Settings*/
	CP_Settings_StrokeWeight(0.5f);

	/*Initializations*/
	cellSize = (float)(CP_System_GetWindowHeight()/SOKOBAN_GRID_ROWS);
	cellAlign = (float)((CP_System_GetWindowWidth()-(int)cellSize*SOKOBAN_GRID_COLS)/2);
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
	if (dir > 0) {
		getCell(playerPosX, playerPosY, dir, grid);
		
	}


	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize*(float)col+cellAlign; 
			float cellY = cellSize*(float)row;
			
			
			draw_floor(cellX,cellY,cellSize);
			
			if (currCell.boarder || currCell.box || currCell.key || currCell.player) {
				if (currCell.boarder)
					draw_boarder(cellX,cellY,cellSize);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX,cellY,cellSize);

				else if (currCell.key) 
					draw_key(cellX,cellY,cellSize);
						
				else if (currCell.player)
					draw_player(cellX,cellY,face);
				
				else if (currCell.box)
					draw_box(cellX,cellY,cellSize);

			}
			
		}
	}
}

void base_Exit(void) {
	free_sprite();
}
