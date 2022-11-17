#pragma once
#include "structs.h"
#include "defines.h"

extern int global_move, move_limit;

void save_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

void undo_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
