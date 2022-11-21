#pragma once
/*
* All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: movement.h
Purpose	: Declaration of extern global_move & move_limit & Functions save_move() & undo_move()
*/

#include "structs.h"					// Needed for Grid struct
#include "defines.h"					// Needed for define Values

extern int global_move, move_limit;		// Allow extern variable call of global_move & move_limit

/* save_move() saves the previous stae of grid after movement */
void save_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

/* undo_move undo one movement by assigning struct array with values from preious state saved by save_move() */
void undo_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
