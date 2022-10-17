#pragma once
#include "structs.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

union CP_Vector AngleToVector(float radian_angle);

int getDirection(void);

void getCell(int posX, int posY, int direction, Cell grid[50][50]);

void gameLogic(int posX, int posY, int nextPosX, int nextPosY, int prevPosX, int prevPosY, Cell grid[50][50]);