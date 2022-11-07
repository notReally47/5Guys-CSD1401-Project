#include "CProcessing.h"
#include "defines.h"
#include "structs.h"
#include "mechanics.h"
#include <stdio.h>
#include <stdlib.h>

/*
* customerLogic - Checks if the cell where the customer is about to move to is valid.
* int cusNum: Subscript of the customer array.
* int posX, posY: The next cell that the customer is moving towards.
* int prevPosX, prevPosY: The previous cell that the customer was previously at.
* int direction: The direction where the customer moves and is looking at.
* Cell grid: Grid where the customer exists.
* Customer customer: Customer stats.
*/
void customerLogic(int cusNum, int posX, int posY, int prevPosX, int prevPosY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	if (!grid[posX][posY].shelf && !grid[posX][posY].boarder && !grid[posX][posY].box && !grid[posX][posY].customer && !grid[posX][posY].key && !grid[posX][posY].player &&!grid[posX][posY].mecha) {
		if (customer[cusNum].isRandom) {
			/*Limit customer random movement within 5x5 grid*/
			if ((customer[cusNum].ogCusCol - 2 <= posY && posY <= customer[cusNum].ogCusCol + 2) &&
				(customer[cusNum].ogCusRow - 2 <= posX && posX <= customer[cusNum].ogCusRow + 2)) {
				grid[posX][posY].customer = 1;
				grid[prevPosX][prevPosY].customer = 0;

				customer[cusNum].cusRow = posX;
				customer[cusNum].cusCol = posY;
				customer[cusNum].direction = direction;
			}
		}

		else {
			grid[posX][posY].customer = 1;
			grid[prevPosX][prevPosY].customer = 0;

			customer[cusNum].cusRow = posX;
			customer[cusNum].cusCol = posY;
			customer[cusNum].direction = direction;
		}
	}
}

/*
* customerMovement - Calculates the customer movement.
* Cell grid: Grid that the customer exists in.
* int path: Grid where the waypoints is located at.
* Customer customer: Customer stats.
*/
void customerMovement(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	for (int i = 0; i < CUSTOMER_MAX; i++) {
		if (customer[i].isActive && !customer[i].isIdle && !customer[i].isRandom) {
			int posX = customer[i].cusRow;
			int posY = customer[i].cusCol;
			int curr = path[posX][posY];

			int count = CP_System_GetFrameCount();

			if (!curr) {
				curr = customer[i].direction;
			}
			else {
				customer[i].direction = curr;
			}

			if (!(count % CUSTOMER_SPEED)) {
				switch (curr) {
					// Move Up
				case SOKOBAN_UP:
					posX--;
					customerLogic(i, posX, posY, posX + 1, posY, curr, grid, customer);
					break;
					// Move Left
				case SOKOBAN_LEFT:
					posY--;
					customerLogic(i, posX, posY, posX, posY + 1, curr, grid, customer);
					break;
					// Move Down
				case SOKOBAN_DOWN:
					posX++;
					customerLogic(i, posX, posY, posX - 1, posY, curr, grid, customer);
					break;
					// Move Right
				case SOKOBAN_RIGHT:
					posY++;
					customerLogic(i, posX, posY, posX, posY - 1, curr, grid, customer);
					break;
					// Default case (if any)
				default:
					break;
				}
			}
		}
	}
}

/*
* randomCustomerMovement - Randomised the customer movement.
* Cell grid: Grid that the customer exists in.
* Customer customer: Customer stats.
*/
void randomCustomerMovement(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	for (int i = 0; i < CUSTOMER_MAX; i++) {
		if (customer[i].isActive && customer[i].isRandom) {
			int posX = customer[i].cusRow, posY = customer[i].cusCol;
			int state = CP_Random_RangeInt(SOKOBAN_UP, SOKOBAN_FACE_RIGHT);
			if (!(CP_System_GetFrameCount() % CUSTOMER_RANDOM_SPEED)) {
				switch (state) {
				case SOKOBAN_UP:
					posX--;
					customerLogic(i, posX, posY, posX + 1, posY, SOKOBAN_UP, grid, customer);
					break;
				case SOKOBAN_LEFT:
					posY--;
					customerLogic(i, posX, posY, posX, posY + 1, SOKOBAN_LEFT, grid, customer);
					break;
				case SOKOBAN_DOWN:
					posX++;
					customerLogic(i, posX, posY, posX - 1, posY, SOKOBAN_DOWN, grid, customer);
					break;
				case SOKOBAN_RIGHT:
					posY++;
					customerLogic(i, posX, posY, posX, posY - 1, SOKOBAN_RIGHT, grid, customer);
					break;
				case SOKOBAN_FACE_UP:
					customer[i].direction = SOKOBAN_UP;
					break;
				case SOKOBAN_FACE_LEFT:
					customer[i].direction = SOKOBAN_LEFT;
					break;
				case SOKOBAN_FACE_DOWN:
					customer[i].direction = SOKOBAN_DOWN;
					break;
				case SOKOBAN_FACE_RIGHT:
					customer[i].direction = SOKOBAN_RIGHT;
					break;
				default:
					customer[i].direction = SOKOBAN_DOWN;
					break;
				}
			}
		}
	}
}

/*
* customerIdle - Idle AI for customer.
* int cusNum: Array Number for customer.
* Customer customer: customer stats.
*/
void customerIdle(int cusNum, Customer customer[CUSTOMER_MAX]) {
	int count = CP_System_GetFrameCount();

	// Rotates the customer anti-clockwise
	if (customer[cusNum].isActive && !(count % CUSTOMER_TURN)) {
		customer[cusNum].direction = (customer[cusNum].direction % 4) + 1;
	}
}

/*
* customerLock - Checks if a player is within the range of a customer.
* Cell grid: Grid that the customer exists in.
* Customer customer: customer stats.
*/
int customerLock(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	int isLocked = 0;
	for (int i = 0; i < CUSTOMER_MAX; i++) {
		if (customer[i].isActive) {
			switch (customer[i].direction) {
				/* Face up */
			case SOKOBAN_UP:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].cusRow - x][customer[i].cusCol].box || grid[customer[i].cusRow - x][customer[i].cusCol].shelf) {
						break;
					}

					if (grid[customer[i].cusRow - x][customer[i].cusCol].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						if (infected[i] == 1) // from mechanics.h
							infected[10] = 1; // status (if true, player is infected)
						break;
					}
				}

				break;

				/* Face left */
			case SOKOBAN_LEFT:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].cusRow][customer[i].cusCol - x].box || grid[customer[i].cusRow][customer[i].cusCol -x].shelf) {
						break;
					}

					if (grid[customer[i].cusRow][customer[i].cusCol - x].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						if (infected[i] == 1) // from mechanics.h
							infected[10] = 1; // status (if true, player is infected)
						break;
					}
				}
				break;

				/* Face down */
			case SOKOBAN_DOWN:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].cusRow + x][customer[i].cusCol].box || grid[customer[i].cusRow + x][customer[i].cusCol].shelf) {
						break;
					}

					if (grid[customer[i].cusRow + x][customer[i].cusCol].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						if (infected[i] == 1) // from mechanics.h
							infected[10] = 1; // status (if true, player is infected)
						break;
					}
				}
				break;

				/* Face right */
			case SOKOBAN_RIGHT:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].cusRow][customer[i].cusCol + x].box || grid[customer[i].cusRow][customer[i].cusCol + x].shelf) {
						break;
					}

					if (grid[customer[i].cusRow][customer[i].cusCol + x].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						if (infected[i] == 1) // from mechanics.h
							infected[10] = 1; // status (if true, player is infected)
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
	}
	return isLocked;
}