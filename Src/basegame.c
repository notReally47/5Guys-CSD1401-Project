#include "cprocessing.h"
#include "structs.h"
#include "utils.h"
#include "defines.h"

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellWidth, cellHeight;

int totalObjs;

void base_Init(void) {
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

	grid[20][20].key = 1;
	grid[25][25].key = 1;

	totalObjs = 2;

	/*Settings*/
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_StrokeWeight(0.5f);

	/*Initializations*/
	cellWidth = CP_System_GetWindowWidth() / (float)SOKOBAN_GRID_COLS;
	cellHeight = CP_System_GetWindowHeight() / (float)SOKOBAN_GRID_ROWS;
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
		}
	}

	/*If all objectives reached, do something here*/
	if (isCompleted == totalObjs) {
		//CP_Engine_Terminate();
	}

	/*Game logic*/
	if (dir > 0) {
		getCell(playerPosX, playerPosY, dir, grid);
	}

	/*Rendering*/
	CP_Graphics_ClearBackground(WHITE);

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellWidth * col;
			float cellY = cellHeight * row;

			if (currCell.boarder || currCell.box || currCell.key || currCell.player) {
				if (currCell.boarder) 
					CP_Settings_Fill(BLACK);

				else if (currCell.player)
					CP_Settings_Fill(RED);

				else if (currCell.key && currCell.box)
					CP_Settings_Fill(VIOLET);
					
				else if (currCell.box)
					CP_Settings_Fill(BLUEGRAY);

				else if (currCell.key)
					CP_Settings_Fill(YELLOW);

				CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight);
			}
		}
	}
}

void base_Exit(void) {

}
