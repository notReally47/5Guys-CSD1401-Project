#pragma once
#include "structs.h"			// Needed for Grid and Customer Structs
#include "defines.h"			// Needed for define Values
#include "generateLevel.h"		// Needed for setMap() function

/*Counts the number of moves and saves the previous state of the grid to a new array 'moves'*/
int moveCount(int move, Move moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[move][row][col].player = grid[row][col].player;
			moves[move][row][col].box = grid[row][col].box;
			moves[move][row][col].customer = grid[row][col].customer;
		}
	}
	//printf("Current Moves: %d\n", ++move);
	++move;
	return move;
}

/*Sets current 'grid' array to the previous 'moves' array to undo a move, decrement number of moves*/
int undoMove(int move, Move moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	if (move > 1) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				grid[row][col].player = moves[move - 1][row][col].player;
				grid[row][col].box = moves[move - 1][row][col].box;
				grid[row][col].customer = moves[move - 1][row][col].customer;
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
int resetMap(int move, Move moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	for (int map = 0; map < move; map++) {
		for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
			for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
				moves[move][row][col].player = 0;
				moves[move][row][col].box = 0;
			}
		}
	}

	/*Call setMap() to reset the map to original state*/
	setMap(grid, customer);

	/*Reset Move Count to 0*/
	return move = 1;
}
