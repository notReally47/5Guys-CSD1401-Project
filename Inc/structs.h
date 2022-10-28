#pragma once

typedef struct Customer {
	int posX;
	int posY;
	int ogPosX;
	int ogPosY;
	int direction;
	int range;
	int isActive;
	int isIdle;
	int isRandom;
} Customer;

typedef struct Cell {
	int player;
	int key;
	int box;
	int boarder;
	int customer;
	int shelf;
} Cell;
