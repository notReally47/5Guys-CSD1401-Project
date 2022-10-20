#include "cprocessing.h"
#include "structs.h"
#include "utils.h"
#include "defines.h"


Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]; //Move counting array for undo and reset

float cellSize,cellAlign;

int totalObjs, boarder, key, player, box, move;

void base_Init(void) {
<<<<<<< Updated upstream
	// already declared in splash_screen. used for main.c -> basegame.c
	CP_System_SetWindowSize(CP_System_GetDisplayWidth()>>1,CP_System_GetDisplayHeight()>>1);

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
=======
	move = 0; //Initialise move with 0
	setMap(grid); //Initialise map
	totalObjs = getObjective(grid); //Counts number of key objective to meet
>>>>>>> Stashed changes

	/*Settings*/
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_StrokeWeight(0.5f);

	/*Initializations*/
	cellSize = (float)CP_System_GetWindowHeight()*1/(float)SOKOBAN_GRID_ROWS;
	cellAlign = cellSize*0.3f*SOKOBAN_GRID_ROWS; // 0.3f roughly aligns in the middle
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
<<<<<<< Updated upstream
		// return to main menu probably
		// CP_Engine_SetNextGameState(Main_Menu_Init,Main_Menu_Update,Main_Menu_Exit);
=======
		CP_Engine_Terminate();
>>>>>>> Stashed changes
	}

	/*Game logic*/
	if (dir > 0) {
		move = moveCount(move, moves, grid); //Counts move and save current state of grid to a different array 'moves'
		getCell(playerPosX, playerPosY, dir, grid);
	}

	if (CP_Input_KeyTriggered(KEY_U)) {
		move = undoMove(move, moves, grid); //Undo a move and set grid to the previous move based on 'moves' array
	}
	else if (CP_Input_KeyTriggered(KEY_R)) {
		move = resetMap(move, moves, grid); //Resets grid to the initial values based on the CSV file
	}

	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize*col+cellAlign; 
			float cellY = cellSize*row;

			if (currCell.boarder || currCell.box || currCell.key || currCell.player) {
				if (currCell.boarder) 
<<<<<<< Updated upstream
					CP_Settings_Fill(BLACK);

				else if (currCell.player)
					CP_Settings_Fill(RED);
=======
					CP_Settings_Fill(BROWN);

				else if (currCell.player)
					CP_Settings_Fill(GREEN);
>>>>>>> Stashed changes

				else if (currCell.key && currCell.box)
					CP_Settings_Fill(VIOLET);
					
				else if (currCell.box)
<<<<<<< Updated upstream
					CP_Settings_Fill(WHITE);
=======
					CP_Settings_Fill(BLUEGRAY);
>>>>>>> Stashed changes

				else if (currCell.key)
					CP_Settings_Fill(YELLOW);

				CP_Graphics_DrawRect(cellX, cellY, cellSize, cellSize);
			}
		}
	}
}

void base_Exit(void) {

}
