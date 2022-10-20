#define _CRT_SECURE_NO_WARNINGS //Added to use fopen & fscanf instead of fopen_s & fscanf_s
#include "cprocessing.h"
#include "structs.h"
#include "defines.h"
#include "utils.h"
#include <stdio.h> //Added for parsing CSV file
#include <errno.h> //Added for error handling/checking of parsing CSV file

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// finding X1,Y1 coords -- top left corner of the rectangle
	float x1 = area_center_x - area_width * 0.5f;
	float y1 = area_center_y - area_height * 0.5f;
	// finding X2,Y2 coords -- bottom right corner of the rectangle
	float x2 = area_center_x + area_width * 0.5f;
	float y2 = area_center_y + area_height * 0.5f;
	return ((click_x<x2&& click_x>x1 && click_y<y2&& click_y>y1) ? 1 : 0); // return 1 if the mouse is within the rectangle
}

/*
* getDirection - returns the direction by input.
*/
int getDirection(void) {
	static float delay = 0.f;
	delay += CP_System_GetDt();

	if (delay >= 0.08f) {
		delay = 0.f;
		if (CP_Input_KeyDown(KEY_W))
			return SOKOBAN_UP;

		else if (CP_Input_KeyDown(KEY_A))
			return SOKOBAN_LEFT;

		else if (CP_Input_KeyDown(KEY_S))
			return SOKOBAN_DOWN;

		else if (CP_Input_KeyDown(KEY_D))
			return SOKOBAN_RIGHT;
		
	}
	

	return SOKOBAN_IDLE;
}

/*
* gameLogic - Logic and interactions between players, boxes, objective and boarders. 
* int posX, posY: The next cell that the player is moving towards.
* int nextPosX, nextPosY: The following cell after the next cell.
* int prevPosX, prevPosY: The previous cell that the player was previously at.
*/
void gameLogic(int posX, int posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	/*Push box (No boarder or another box blocking the box being pushed)*/
	if (grid[posX][posY].box && !grid[nextPosX][nextPosY].box && !grid[posX][posY].boarder && !grid[nextPosX][nextPosY].boarder) {
		grid[prevPosX][prevPosY].player = 0;
		grid[posX][posY].player = 1;
		grid[posX][posY].box = 0;
		grid[nextPosX][nextPosY].box = 1;
	}

	/*Player movement without obstruction*/
	else if (!grid[posX][posY].box && !grid[posX][posY].boarder) {
		grid[prevPosX][prevPosY].player = 0;
		grid[posX][posY].player = 1;
	}
}

void getCell(int posX, int posY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	switch (direction) {
	/* Move up */
	case 1:
		--posX;
		gameLogic(posX, posY, posX - 1, posY, posX + 1, posY, grid);
		break;

	/* Move left */
	case 2:
		--posY;
		gameLogic(posX, posY, posX, posY - 1, posX, posY + 1, grid);
		break;

	/* Move down */
	case 3:
		++posX;
		gameLogic(posX, posY, posX + 1, posY, posX - 1, posY, grid);
		break;

	/* Move right */
	case 4:
		++posY;
		gameLogic(posX, posY, posX, posY + 1, posX, posY - 1, grid);
		break;

	/* Default case (if any) */
	default:
		break;
	}
}

<<<<<<< Updated upstream
/*
* timer - timer takes in the duration given (seconds) and returns the duration left.
* duration - Seconds given to complete the level
* startTime - Current time when loaded into the game state (declare startTime during the Init of level)
*/
int timer(int duration, float startTime) {
	int diffTime = (int)CP_System_GetSeconds() - (int)startTime;
	int remaindingTime = duration - diffTime;

	if (!(remaindingTime < 0)) {
		return remaindingTime;
	}
	else {
		return 0;
	}
}
=======
/*Parse CSV file to initialise grid array at the start of every stage/level*/
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	printf("In setMap! \n");
	int boarder = 0, key = 0, player = 0, box = 0, row = 0, col = 0, read = 0, records = 0;
	FILE* csvFile;
	char file[] = "..\\..\\Extern\\level_mapper\\test_level.csv";
	csvFile = 0;
	csvFile =  fopen(file,"r");

	if (NULL == csvFile) {
		printf("%s \n", &file);
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(1);
	}
	do {
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}
		read = fscanf(csvFile, "%d,%d,%d,%d\n", &grid[row][col].boarder, &grid[row][col].key, &grid[row][col].player, &grid[row][col].box);
		printf("row is %d, col is %d, boarder is %d \n", row, col, grid[row][col].boarder);
		if (read == 4) col++;
		
		if (read != 4 && !feof(csvFile)) {
			printf("File format incorrect. \n");
		}
		if (ferror(csvFile)) {
			printf("Error reading file.\n");
		}
	} while (!feof(csvFile));

	fclose(csvFile);
}

/*Counts the number of key objective points in the grid and returns it*/
int getObjective(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	int objective = 0;
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			if (grid[row][col].key) objective++;
		}
	}
	return objective;
}

/*Counts the number of moves and saves the previous state of the grid to a new array 'moves'*/
int moveCount(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[move][row][col] = grid[row][col];
		}
	}
	printf("Current Moves: %d\n", ++move);
	return move;
}

/*Sets current 'grid' array to the previous 'moves' array to undo a move, decrement number of moves*/
int undoMove(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (move > 0) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				grid[row][col] = moves[move-1][row][col];
				moves[move-1][row][col].boarder = 0;
				moves[move-1][row][col].key = 0;
				moves[move-1][row][col].player = 0;
				moves[move-1][row][col].box = 0;
			}
		}
		printf("Current Moves: %d\n", --move);
	}
	return move;
}

/*Resets grid to the initial state based on the CSV, resets move to 0*/
int resetMap(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	for (int map = 0; map < move; map++) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				moves[move][row][col].boarder = 0;
				moves[move][row][col].key = 0;
				moves[move][row][col].player = 0;
				moves[move][row][col].box = 0;
			}
		}
	}
	setMap(grid);
	return move = 0;
}
>>>>>>> Stashed changes
