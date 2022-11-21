#pragma once
/*
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: level_generate.h
Purpose	: Declaration of extern original_box_count & Function set_map()
*/

#include "defines.h"		// Needed for defined values
#include "structs.h"		// Needed for defined structs

extern original_box_count;	// Allow extern variable call of original_box_count

/* set_map() loads level CSV file & load game map */
void set_map(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX],
	int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]);
