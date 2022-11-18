#pragma once
#include "structs.h"
#include "defines.h"
extern int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
void base_Init(void);
void base_Update(void);
void base_Exit(void);