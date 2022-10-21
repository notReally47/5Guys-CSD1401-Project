#include "CProcessing.h"
#include "defines.h"
#include "structs.h"
#include <stdio.h>

void customerLogic(int posX, int posY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (!grid[posX][posY].boarder && !grid[posX][posY].box && !grid[posX][posY].customer.isCustomer && !grid[posX][posY].key && !grid[posX][posY].player) {
		Customer nullCus;
		nullCus.posX = 0;
		nullCus.posY = 0;
		nullCus.direction = 0;
		nullCus.range = 0;
		nullCus.isCustomer = 0;
		nullCus.isActive = 0;
		grid[posX][posY].customer = grid[prevPosX][prevPosY].customer;
		grid[prevPosX][prevPosY].customer = nullCus;
	}
}

void customerMovement(int posX, int posY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (grid[posX][posY].customer.isActive) {
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
			//todo change direction when fella moves
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
}

/*
* customerLock - Checks if a customer is within the range of a customer
* int posX: X position of customer
* int posY: Y position of customer
* Cell grid: Grid that the customer exists in
*/
int customerLock(int posX, int posY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	int isLocked = 0;
	if (grid[posX][posY].customer.isActive) {
		switch (grid[posX][posY].customer.direction) {
			/* Face up */
		case SOKOBAN_UP:
			for (int i = 1; i <= grid[posX][posY].customer.range; i++) {
				if (grid[posX - i][posY].player) {
					isLocked = 1;
					break;
				}
			}

			break;

			/* Face left */
		case SOKOBAN_LEFT:
			for (int i = 1; i <= grid[posX][posY].customer.range; i++) {
				if (grid[posX][posY - i].player) {
					isLocked = 1;
					break;
				}
			}
			break;

			/* Face down */
		case SOKOBAN_DOWN:
			for (int i = 1; i <= grid[posX][posY].customer.range; i++) {
				if (grid[posX + i][posY].player) {
					isLocked = 1;
					break;
				}
			}
			break;

			/* Face right */
		case SOKOBAN_RIGHT:
			for (int i = 1; i <= grid[posX][posY].customer.range; i++) {
				if (grid[posX][posY + i].player) {
					isLocked = 1;
					break;
				}
			}
			break;

			/* Default case (if any) */
		default:
			isLocked = 0;
			break;
		}
	}

	return isLocked;
}