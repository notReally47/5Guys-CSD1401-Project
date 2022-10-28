#pragma once
#include "structs.h"
#include "defines.h"
#include "generateLevel.h"

/*Counts the number of moves and saves the previous state of the grid to a new array 'moves'*/
int moveCount(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[move][row][col] = grid[row][col];
		}
	}
	//printf("Current Moves: %d\n", ++move);
	++move;
	return move;
}

/*Sets current 'grid' array to the previous 'moves' array to undo a move, decrement number of moves*/
int undoMove(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (move > 0) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				grid[row][col] = moves[move - 1][row][col];
				moves[move - 1][row][col].boarder = 0;
				moves[move - 1][row][col].key = 0;
				moves[move - 1][row][col].player = 0;
				moves[move - 1][row][col].box = 0;
			}
		}
		//printf("Current Moves: %d\n", --move);
		--move;
	}
	return move;
}

/*Resets grid to the initial state based on the CSV, resets move to 0*/
int resetMap(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	for (int map = 0; map < move; map++) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				moves[move][row][col].boarder = 0;
				moves[move][row][col].key = 0;
				moves[move][row][col].player = 0;
				moves[move][row][col].box = 0;
			}
		}
	}

	setMap(grid, customer);
	return move = 0;
}
