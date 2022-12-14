#pragma once
#include "defines.h"

void customerMovement(int cusNum, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]);
void randomCustomerMovement(int cusNum, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]);
int customerLock(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]);
void customerIdle(int cusNum, Customer customer[CUSTOMER_MAX]);
int customerMoveToPlayer(int playerRow, int playerCol, int cusNum, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX]);