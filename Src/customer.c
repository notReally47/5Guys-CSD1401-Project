#include "CProcessing.h"
#include "defines.h"
#include "structs.h"
#include <stdio.h>

void customerLogic(int posX, int posY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (!grid[posX][posY].boarder && !grid[posX][posY].box && !grid[posX][posY].customer && !grid[posX][posY].key && !grid[posX][posY].player) {
		grid[prevPosX][prevPosY].customer = 0;
		grid[posX][posY].customer = 1;
	}
}

void customerMovement(int posX, int posY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	static int moveState;
	int curr = path[posX][posY];
	int count = CP_System_GetFrameCount();

	if (!curr) {
		curr = moveState;
	}
	else {
		moveState = curr;
	}
	
	if (!(count % 10)) {
		switch (curr) {
		case 1:
			posX--;
			customerLogic(posX, posY, posX + 1, posY, grid);
			break;
		case 2:
			posY--;
			customerLogic(posX, posY, posX, posY + 1, grid);
			break;
		case 3:
			posX++;
			customerLogic(posX, posY, posX - 1, posY, grid);
			break;
		case 4:
			posY++;
			customerLogic(posX, posY, posX, posY - 1, grid);
			break;
		default:
			break;
		}
	}
}
