#pragma once

#include "defines.h"
#include "structs.h"

extern original_box_count;

void set_map(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX],
	int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]);
