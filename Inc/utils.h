#pragma once
#include "structs.h"
#include "defines.h"

extern int duration;
extern float lockTimer;

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int getDirection(void);
int collisionCheck(int posX, int posY, int moveX, int moveY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
void getCell(int *posX, int *posY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
void gameLogic(int *posX, int *posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
int timer(int duration, float startTime);
int getObjective(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
