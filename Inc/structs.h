#pragma once

typedef struct Customer {
	int posX;
	int posY;
	int direction;
	int range;
	int isActive;
} Customer;

typedef struct Cell {
	int player;
	int key;
	int box;
	int boarder;
	int customer;
	int shelf;
} Cell;
