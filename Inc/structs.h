#pragma once

typedef struct Customer {
	int posX;
	int posY;
	int direction;
	int range;
	int isCustomer;
	int isActive;
} Customer;

typedef struct Cell {
	int player;
	int key;
	int box;
	int boarder;
	Customer customer;
	int shelf;
} Cell;
