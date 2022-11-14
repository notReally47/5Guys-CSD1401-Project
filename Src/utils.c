#define _CRT_SECURE_NO_WARNINGS		// Needed to use strcpy() and strcat() instead of strcopy_s() & strcat_s()
#define _CRT_SECURE_NO_DEPRECATE	// Needed to use strcpy() and strcat() instead of strcopy_s() & strcat_s()
#include "cprocessing.h"	
#include "structs.h"
#include "defines.h"
#include "utils.h"
#include "movement.h"
#include "mechanics.h"
#include "mainmenu.h"
#include <stdio.h>
#include <stdlib.h>

int duration = 60;
float lockTimer = 3.f;
CP_Sound teleport_sound = NULL;

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// finding X1,Y1 coords -- top left corner of the rectangle
	float x1 = area_center_x - area_width * 0.5f;
	float y1 = area_center_y - area_height * 0.5f;
	// finding X2,Y2 coords -- bottom right corner of the rectangle
	float x2 = area_center_x + area_width * 0.5f;
	float y2 = area_center_y + area_height * 0.5f;
	return ((click_x < x2&& click_x > x1 && click_y < y2&& click_y > y1) ? 1 : 0); // return 1 if the mouse is within the rectangle
}

/*
* getDirection - returns the direction by input.
*/
int getDirection(void) {
	static int key;
	static float delay = 0.f;
	delay += CP_System_GetDt();
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
	if ((CP_Input_KeyDown(key) && delay > (10.f / CP_System_GetFrameRate())) || (CP_Input_KeyTriggered(key) && delay > (10.f / CP_System_GetFrameRate()))) {
		delay = 0.f;
		if (CP_Input_KeyDown(key))
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
	return SOKOBAN_IDLE;
}

/*returns TRUE if valid move is allowed, FALSE if illegal move*/
int collisionCheck(int posX, int posY, int moveX, int moveY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {

	int newPosX = posX + moveX, newPosY = posY + moveY;
	/*Check if tile directly after has obstructions*/
	if (!grid[newPosX][newPosY].boarder &&
		!grid[newPosX][newPosY].customer &&
		!grid[newPosX][newPosY].shelf &&
		!grid[newPosX][newPosY].mecha)
	{
		/*If player is pushing a box, check if theres anything occupying the tile after*/
		if (grid[newPosX][newPosY].box) {
			int nextPosX = newPosX + moveX, nextPosY = newPosY + moveY;

			if (!grid[nextPosX][nextPosY].box &&
				!grid[nextPosX][nextPosY].boarder &&
				!grid[nextPosX][nextPosY].customer &&
				!grid[nextPosX][nextPosY].shelf &&
				!grid[nextPosX][nextPosY].mecha)
			{
				return TRUE;
			}
		}
		/*Player is not pushing a box*/
		else if (!grid[newPosX][newPosY].box) {
			return TRUE;
		}
	}

	return FALSE;
}

/*
* gameLogic - Logic and interactions between players, boxes, objective and boarders.
* int posX, posY: The next cell that the player is moving towards.
* int nextPosX, nextPosY: The following cell after the next cell.
* int prevPosX, prevPosY: The previous cell that the player was previously at.
*/
int gameLogic(int* posX, int* posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]) {
	/*Push box (No boarder or another box blocking the box being pushed)*/
	if (grid[*posX][*posY].box) {
		grid[prevPosX][prevPosY].player = 0;
		grid[*posX][*posY].player = 1;
		grid[nextPosX][nextPosY].box = grid[*posX][*posY].box;
		grid[*posX][*posY].box = 0;
		global_move++;
		printf("Current Moves: %d\n", global_move - 1);
		return grid[nextPosX][nextPosY].key ? 2 : 1;
	}

	/*Player movement without obstruction*/
	else if (!grid[*posX][*posY].box) {
		grid[prevPosX][prevPosY].player = 0;

		if (teleporter[0] == 1 && grid[*posX][*posY].tele != 0) {
			int tele = grid[*posX][*posY].tele - 1;												// Teleporter Number is one more than the array element
			if ((grid[*posX][*posY].tele % 2) == 1) {											// If The Teleporter Number is an Odd Number
				if (*posX == teleporters[tele].posY && *posY == teleporters[tele].posX) {		// If Next Position matches the Teleporter Position
					CP_Sound_PlayAdvanced(teleport_sound, 1, 1, FALSE, CP_SOUND_GROUP_SFX);		// Play Teleporter Sound Effect
					*posX = teleporters[tele + 1].posY + (*posX - prevPosX);					// Move Player row to the Next Even Numbered Teleporter Row
					*posY = teleporters[tele + 1].posX + (*posY - prevPosY);					// Move Player col to the Next Even Numbered Teleporter col
					teleporter[1] = 1;															// Set Teleporter Cooldown to 1
				}
			}
			else {																				// Else if Teleporter Number is an Even Number
				if (*posX == teleporters[tele].posY && *posY == teleporters[tele].posX) {		// If Next Position matches the Teleporter Position
					CP_Sound_PlayAdvanced(teleport_sound, 1, 1, FALSE, CP_SOUND_GROUP_SFX);		// Play Teleporter Sound Effect
					*posX = teleporters[tele - 1].posY + (*posX - prevPosX);					// Move Player row to the Previous Odd Numbered Teleporter Row
					*posY = teleporters[tele - 1].posX + (*posY - prevPosY);					// Move Player row to the Previous Odd Numbered Teleporter Row
					teleporter[1] = 1;															// Set Teleporter Cooldown to 1
				}
				CP_Sound_Free(&teleport_sound);
			}
		}

		//if (teleporter[0] == 1) {
		//	if (*posX == teleporter[1] && *posY == teleporter[2]) {
		//		*posX = teleporter[3] + (*posX - prevPosX);
		//		*posY = teleporter[4] + (*posY - prevPosY);
		//		teleporter[9] = 1;
		//	}
		//	else if (*posX == teleporter[3] && *posY == teleporter[4]) {
		//		*posX = teleporter[1] + (*posX - prevPosX);
		//		*posY = teleporter[2] + (*posY - prevPosY);
		//		teleporter[9] = 1;
		//	}
		//}
		grid[*posX][*posY].player = 1;
		global_move++;
		return 0;
		printf("Current Moves: %d\n", global_move - 1);
	}
}

int getCell(int* posX, int* posY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]) {
	int pushBox = 0;
	switch (direction) {
		/* Move up */
	case 1:
		if (collisionCheck(*posX, *posY, -1, 0, grid)) {
			--* posX;
			pushBox = gameLogic(posX, posY, *posX - 1, *posY, *posX + 1, *posY, grid, teleporters);
		}
		break;

		/* Move left */
	case 2:
		if (collisionCheck(*posX, *posY, 0, -1, grid)) {
			--* posY;
			pushBox = gameLogic(posX, posY, *posX, *posY - 1, *posX, *posY + 1, grid, teleporters);
		}
		break;

		/* Move down */
	case 3:
		if (collisionCheck(*posX, *posY, 1, 0, grid)) {
			++* posX;
			pushBox = gameLogic(posX, posY, *posX + 1, *posY, *posX - 1, *posY, grid, teleporters);
		}
		break;

		/* Move right */
	case 4:
		if (collisionCheck(*posX, *posY, 0, 1, grid)) {
			++* posY;
			pushBox = gameLogic(posX, posY, *posX, *posY + 1, *posX, *posY - 1, grid, teleporters);
		}
		break;

		/* Default case (if any) */
	default:
		break;
	}
	return pushBox;
}

/*
* timer - timer takes in the duration given (seconds) and returns the duration left.
* duration - Seconds given to complete the level
* startTime - Current time when loaded into the game state (declare startTime during the Init of level)
*/
int timer(int duration, float startTime) {
	int diffTime = (int)CP_System_GetSeconds() - (int)startTime;
	int remainingTime = duration - diffTime;
	return (!(remainingTime < 0)) ? remainingTime : 0;
}

/*Counts the number of key objective points in the grid and returns it*/
int getObjective(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	int objective = 0;
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			if (grid[row][col].key) objective++;
		}
	}
	printf("Objectives: %d\n", objective);
	return objective;
}

/* Function to Show Text/Label beside/on the Game */
void show_stats(float text_size, float cell_size_x, float cell_size_h, char* stat, int value) {
	CP_Settings_TextSize(text_size);																												// Set Font Size
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);																			// Align Text Left and Top
	CP_Settings_Fill(BLACK);																														// Set Font to BLACK
	char buffer[20] = { 0 };																														// Buffer to convert 'value' to text/char/string
	char line[50] = { 0 };																															// String to be printed
	if (stat != '\0') {
		if (stat == "Time Left: ") {
			if (value <= 30) {
				(value % 2) ? CP_Settings_Fill(RED) : CP_Settings_Fill(BLACK);
			}
		}
		
		(stat == "Times Distracted: ") ? sprintf_s(buffer, _countof(buffer), "%d", value) : sprintf_s(buffer, _countof(buffer), "%d", value);		// Converting 'value' to text/char/string				
		strcpy(line, stat);																															// Copy stat/label that needs to be printed to line
	}
	strcat(line, buffer);																															// Concatenate line with buffer
	CP_Font_DrawText(line, cell_size_x, cell_size_h);																								// Draw Text 'line' on the Game
}
