#include "cprocessing.h"
#include "structs.h"
#include "defines.h"
#include "utils.h"

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
	static int key;
	static float holdelay = 0.f,delay=0.f;
	delay += holdelay += CP_System_GetDt();
	switch (CP_Input_KeyDown(KEY_ANY)) {
	case 3: 
	case 4: // limits key input to 2
		break; // requires some error catching tests
	default:
		key = (CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyDown(KEY_W) && CP_Input_KeyReleased(KEY_ANY)) ? KEY_W : key; // W
		key = (CP_Input_KeyTriggered(KEY_A) || CP_Input_KeyDown(KEY_A) && CP_Input_KeyReleased(KEY_ANY)) ? KEY_A : key; // A
		key = (CP_Input_KeyTriggered(KEY_S) || CP_Input_KeyDown(KEY_S) && CP_Input_KeyReleased(KEY_ANY)) ? KEY_S : key; // S
		key = (CP_Input_KeyTriggered(KEY_D) || CP_Input_KeyDown(KEY_D) && CP_Input_KeyReleased(KEY_ANY)) ? KEY_D : key; // D
		break;
	}
	if (holdelay >= 0.5f || (CP_Input_KeyTriggered(KEY_ANY) && delay>2.f)) {
		holdelay = 0.f;
		if (CP_Input_KeyDown(key) || CP_Input_KeyTriggered(key)) {
			if (CP_Input_KeyTriggered(key))
				delay = 0.f;
			switch (key) {
			case KEY_W:
				return SOKOBAN_UP;
			case KEY_A:
				return SOKOBAN_LEFT;
			case KEY_S:
				return SOKOBAN_DOWN;
			case KEY_D:
				return SOKOBAN_RIGHT;
			}
		}
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
	if (grid[posX][posY].box && !grid[nextPosX][nextPosY].box && !grid[posX][posY].boarder && !grid[nextPosX][nextPosY].boarder && !grid[posX][posY].customer && !grid[nextPosX][nextPosY].customer) {
		grid[prevPosX][prevPosY].player = 0;
		grid[posX][posY].player = 1;
		grid[posX][posY].box = 0;
		grid[nextPosX][nextPosY].box = 1;
	}

	/*Player movement without obstruction*/
	else if (!grid[posX][posY].box && !grid[posX][posY].boarder && !grid[posX][posY].customer) {
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
