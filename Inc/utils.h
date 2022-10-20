#pragma once
#include "structs.h"
#include "defines.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

//int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

union CP_Vector AngleToVector(float radian_angle);

int getDirection(void);

void getCell(int posX, int posY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

void gameLogic(int posX, int posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

<<<<<<< Updated upstream
int timer(int duration, float startTime);
=======
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

int getObjective(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

int moveCount(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

int undoMove(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);

int resetMap(int move, Cell moves[MOVE][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
>>>>>>> Stashed changes
