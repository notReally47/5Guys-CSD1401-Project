#pragma once
#include "structs.h"
#include "defines.h"

extern int duration;
extern float lockTimer;

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int getDirection(void);
int collisionCheck(int posX, int posY, int moveX, int moveY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
int getCell(int *posX, int *posY, int direction, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]);
int gameLogic(int *posX, int *posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]);
int timer(int duration, float startTime);
int get_objectives(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]);
void show_stats(float cellSize, char* stat, int value, int cameratoggle, float index);
BOOL IsTaskbarWndVisible(void);
int getTaskBarHeight(void);
