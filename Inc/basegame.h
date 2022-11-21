#pragma once
/*
Author	: All Members
File	: basegame.h
Purpose	: State for Main Gameplay
*/

#include "structs.h"	// Needed for Game Structs
#include "defines.h"	// Needed for Defined Values

extern int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];	// Allow extern array of path[][]
void base_Init(void);
void base_Update(void);
void base_Exit(void);