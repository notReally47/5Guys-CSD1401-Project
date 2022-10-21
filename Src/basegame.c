#include "cprocessing.h"
#include "structs.h"
#include "utils.h"
#include "defines.h"
#include "customer.h"
#include <stdio.h>

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellSize,cellAlign, sec;

int totalObjs;

void base_Init(void) {
	// already declared in splash_screen. used for main.c -> basegame.c
	CP_System_SetWindowSize(CP_System_GetDisplayWidth()>>1,CP_System_GetDisplayHeight()>>1);

	/*Create empty grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			grid[row][col].boarder = 0;
			grid[row][col].key = 0;
			grid[row][col].player = 0;
			grid[row][col].box = 0;
			path[row][col] = 0;
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

	grid[20][10].customer.isCustomer = 1;

	path[20][10] = 1;	// Customer waypoint to go Up
	path[15][10] = 2;	// Customer waypoint to go Left
	path[15][5] = 3;	// Customer waypoint to go Down
	path[20][5] = 4;	// Customer waypoint to go Right

	totalObjs = 2;

	/*Settings*/
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_StrokeWeight(0.5f);

	/*Initializations*/
	cellSize = (float)CP_System_GetWindowHeight() * 1 / (float)SOKOBAN_GRID_ROWS;
	cellAlign = cellSize * 0.3f * SOKOBAN_GRID_ROWS; // 0.3f roughly aligns in the middle

	sec = CP_System_GetSeconds();
	CP_System_SetFrameRate(60.0f);
}

void base_Update(void) {
	/*Check for input and get the direction of the input*/
	int dir = getDirection();

	int playerPosX, playerPosY, customerPosX, customerPosY, isCompleted = 0;

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

			if (grid[row][col].customer.isCustomer) {
				customerPosX = row;
				customerPosY = col;
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
	
	customerMovement(customerPosX, customerPosY, grid, path);

	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize*col+cellAlign; 
			float cellY = cellSize*row;

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.customer.isCustomer) {
				if (currCell.boarder)
					CP_Settings_Fill(BLACK);

				else if (currCell.player)
					CP_Settings_Fill(RED);

				else if (currCell.key && currCell.box)
					CP_Settings_Fill(VIOLET);

				else if (currCell.box)
					CP_Settings_Fill(WHITE);

				else if (currCell.key)
					CP_Settings_Fill(YELLOW);

				else if (currCell.customer.isCustomer)
					CP_Settings_Fill(PINK);

				CP_Graphics_DrawRect(cellX, cellY, cellSize, cellSize);
			}
		}
	}
}

void base_Exit(void) {

}
