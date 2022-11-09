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
void customerLogic(int cusNum, int cusRow, int cusCol, int prevCusRow, int prevCusCol, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	if (!grid[cusRow][cusCol].shelf && !grid[cusRow][cusCol].boarder && !grid[cusRow][cusCol].box && !grid[cusRow][cusCol].customer && !grid[cusRow][cusCol].key && !grid[cusRow][cusCol].player && !grid[cusRow][cusCol].mecha) {
		if (customer[cusNum].isRandom) {
			/*Limit customer random movement within 5x5 grid*/
			if ((customer[cusNum].ogCusCol - 2 <= cusCol && cusCol <= customer[cusNum].ogCusCol + 2) &&
				(customer[cusNum].ogCusRow - 2 <= cusRow && cusRow <= customer[cusNum].ogCusRow + 2)) {
				grid[cusRow][cusCol].customer = 1;
				grid[prevCusRow][prevCusCol].customer = 0;

				customer[cusNum].cusRow = cusRow;
				customer[cusNum].cusCol = cusCol;
				customer[cusNum].direction = direction;
			}
		}

		else {
			grid[cusRow][cusCol].customer = 1;
			grid[prevCusRow][prevCusCol].customer = 0;

			customer[cusNum].cusRow = cusRow;
			customer[cusNum].cusCol = cusCol;
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
	static float delay = 0.f;
	delay += CP_System_GetDt();

	if (delay > 1.5f) {
		delay = 0.f;
		for (int i = 0; i < CUSTOMER_MAX; i++) {
			if (customer[i].isActive && !customer[i].isIdle && !customer[i].isRandom) {
				int cusRow = customer[i].cusRow;
				int cusCol = customer[i].cusCol;
				int curr = path[cusRow][cusCol];

				if (!curr) {
					curr = customer[i].direction;
				}
				else {
					customer[i].direction = curr;
				}
				switch (curr) {
					// Move Up
				case SOKOBAN_UP:
					cusRow--;
					customerLogic(i, cusRow, cusCol, cusRow + 1, cusCol, curr, grid, customer);
					break;
					// Move Left
				case SOKOBAN_LEFT:
					cusCol--;
					customerLogic(i, cusRow, cusCol, cusRow, cusCol + 1, curr, grid, customer);
					break;
					// Move Down
				case SOKOBAN_DOWN:
					cusRow++;
					customerLogic(i, cusRow, cusCol, cusRow - 1, cusCol, curr, grid, customer);
					break;
					// Move Right
				case SOKOBAN_RIGHT:
					cusCol++;
					customerLogic(i, cusRow, cusCol, cusRow, cusCol - 1, curr, grid, customer);
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
			int cusRow = customer[i].cusRow, cusCol = customer[i].cusCol;
			int state = CP_Random_RangeInt(SOKOBAN_UP, SOKOBAN_FACE_RIGHT);
			if (!(CP_System_GetFrameCount() % CUSTOMER_RANDOM_SPEED)) {
				switch (state) {
				case SOKOBAN_UP:
					cusRow--;
					customerLogic(i, cusRow, cusCol, cusRow + 1, cusCol, SOKOBAN_UP, grid, customer);
					break;
				case SOKOBAN_LEFT:
					cusCol--;
					customerLogic(i, cusRow, cusCol, cusRow, cusCol + 1, SOKOBAN_LEFT, grid, customer);
					break;
				case SOKOBAN_DOWN:
					cusRow++;
					customerLogic(i, cusRow, cusCol, cusRow - 1, cusCol, SOKOBAN_DOWN, grid, customer);
					break;
				case SOKOBAN_RIGHT:
					cusCol++;
					customerLogic(i, cusRow, cusCol, cusRow, cusCol - 1, SOKOBAN_RIGHT, grid, customer);
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
			int cusRow = customer[i].cusRow;
			int cusCol = customer[i].cusCol;

			switch (customer[i].direction) {
				/* Face up */
			case SOKOBAN_UP:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[cusRow - x][cusCol].box || grid[cusRow - x][cusCol].shelf) {
						break;
					}

					if (grid[cusRow - x][cusCol].player) {
						isLocked = i + 1;
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
					if (grid[cusRow][cusCol - x].box || grid[cusRow][cusCol - x].shelf) {
						break;
					}

					if (grid[cusRow][cusCol - x].player) {
						isLocked = i + 1;
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
					if (grid[cusRow + x][cusCol].box || grid[cusRow + x][cusCol].shelf) {
						break;
					}

					if (grid[cusRow + x][cusCol].player) {
						isLocked = i + 1;
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
					if (grid[cusRow][cusCol + x].box || grid[cusRow][cusCol + x].shelf) {
						break;
					}

					if (grid[cusRow][cusCol + x].player) {
						isLocked = i + 1;
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

int customerMoveToPlayer(int playerRow, int playerCol, int cusNum, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]) {
	int face = 0;
	int cusRow = customer[cusNum].cusRow;
	int cusCol = customer[cusNum].cusCol;
	int direction = customer[cusNum].direction;

	if (!customer[cusNum].isActive) {
		customer[cusNum].isRandom = 0;
		switch (direction) {
		case SOKOBAN_UP:
			if (cusRow != playerRow - 1) {
				cusRow--;
				customerLogic(cusNum, cusRow, cusCol, cusRow + 1, cusCol, direction, grid, customer);
				face = 3;
			}
			break;
		case SOKOBAN_LEFT:
			if (cusCol != playerCol - 1) {
				cusCol--;
				customerLogic(cusNum, cusRow, cusCol, cusRow, cusCol + 1, direction, grid, customer);
				face = 4;
			}
			break;
		case SOKOBAN_DOWN:
			if (cusRow != playerRow + 1) {
				cusRow++;
				customerLogic(cusNum, cusRow, cusCol, cusRow - 1, cusCol, direction, grid, customer);
				face = 1;
			}
			break;
		case SOKOBAN_RIGHT:
			if (cusRow != playerCol + 1) {
				cusCol++;
				customerLogic(cusNum, cusRow, cusCol, cusRow, cusCol - 1, direction, grid, customer);
				face = 2;
			}
			break;
			// Default case (if any)
		default:
			break;
		}
	}
	return face;
}
