#include "CProcessing.h"
#include "defines.h"
#include "structs.h"
#include <stdio.h>

/*
* customerLogic - Checks if the cell where the customer is about to move to is valid.
* int cusNum: Subscript of the customer array.
* int posX, posY: The next cell that the customer is moving towards.
* int prevPosX, prevPosY: The previous cell that the customer was previously at.
* Cell grid: Grid where the customer exists.
* Customer customer: Customer stats.
*/
void customerLogic(int cusNum, int posX, int posY, int prevPosX, int prevPosY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	if (!grid[posX][posY].boarder && !grid[posX][posY].box && !grid[posX][posY].customer && !grid[posX][posY].key && !grid[posX][posY].player) {

		grid[posX][posY].customer = 1;
		grid[prevPosX][prevPosY].customer = 0;
		
		customer[cusNum].posX = posX;
		customer[cusNum].posY = posY;
	}
}

/*
* customerMovement - Calculates the customer movement.
* Cell grid: Grid that the customer exists in.
* int path: Grid where the waypoints is located at.
* Customer customer: Customer stats.
*/
void customerMovement(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	for (int i = 0; i < CUSTOMER; i++) {
		if (customer[i].isActive) {
			int posX = customer[i].posX;
			int posY = customer[i].posY;
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
				case SOKOBAN_UP:
					posX--;
					customerLogic(i, posX, posY, posX + 1, posY, curr, grid, customer);
					break;
				case SOKOBAN_LEFT:
					posY--;
					customerLogic(i, posX, posY, posX, posY + 1, curr, grid, customer);
					break;
				case SOKOBAN_DOWN:
					posX++;
					customerLogic(i, posX, posY, posX - 1, posY, curr, grid, customer);
					break;
				case SOKOBAN_RIGHT:
					posY++;
					customerLogic(i, posX, posY, posX, posY - 1, curr, grid, customer);
					break;
				default:
					break;
				}
			}
		}
	}
}

/*
* customerLock - Checks if a player is within the range of a customer.
* Cell grid: Grid that the customer exists in.
* Customer customer: customer stats.
*/
int customerLock(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	int isLocked = 0;
	for (int i = 0; i < CUSTOMER; i++) {
		if (customer[i].isActive) {
			switch (customer[i].direction) {
				/* Face up */
			case SOKOBAN_UP:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].posX - x][customer[i].posY].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						break;
					}
				}

				break;

				/* Face left */
			case SOKOBAN_LEFT:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].posX][customer[i].posY - x].player) {
						isLocked = 1;
						customer[i].isActive = 0;
						break;
					}
				}
				break;

				/* Face down */
			case SOKOBAN_DOWN:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].posX + x][customer[i].posY].player) {
						isLocked = 1;
						customer[i].isActive = 0;;
						break;
					}
				}
				break;

				/* Face right */
			case SOKOBAN_RIGHT:
				for (int x = 1; x <= customer[i].range; x++) {
					if (grid[customer[i].posX][customer[i].posY + x].player) {
						isLocked = 1;
						customer[i].isActive = 0;
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