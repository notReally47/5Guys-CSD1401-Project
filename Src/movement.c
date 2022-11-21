#pragma once
/*
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: movement.c
Purpose	: Functions to save move states & undo moves
*/

#include "structs.h"			// Needed for Grid struct
#include "defines.h"			// Needed for define Values

int global_move, move_limit;	// The global variable for move counting & move limit, to be used for different funtions & logics

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
	}

}
