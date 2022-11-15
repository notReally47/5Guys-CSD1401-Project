#pragma once
#include "structs.h"			// Needed for Grid and Customer Structs
#include "defines.h"			// Needed for define Values
#include "level_generate.h"		// Needed for set_map() function
#include <stdio.h>				// Needed for printf()

int global_move;				// The global variable for move counting, to be used for different funtions & logics

/* Counts the number of moves and saves the previous state of the grid to a new array 'moves' */
void save_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[global_move][row][col].player = grid[row][col].player;
			moves[global_move][row][col].box = grid[row][col].box;
		}
	}

}

/* Sets current 'grid' array to the previous 'moves' array to undo a move, decrement number of moves */
void undo_move(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	
	if (global_move > 1) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				grid[row][col].player = moves[global_move - 1][row][col].player;
				grid[row][col].box = moves[global_move - 1][row][col].box;
				moves[global_move - 1][row][col].player = 0;
				moves[global_move - 1][row][col].box = 0;
			}
		}
		global_move--; 
		printf("Undo Move\n");
		printf("Current Moves: %d\n", global_move-1);
	}

}

/* Resets grid to the initial state based on the CSV, resets move to 0 */
void reset_map(Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]) {
	
	for (int map = 0; map < global_move; map++) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				moves[global_move][row][col].player = 0;
				moves[global_move][row][col].box = 0;
			}
		}
	}

	/*Call setMap() to reset the map to original state*/
	set_map(grid, customer, path, teleporters);
	global_move = 1;

}
