#pragma once
#include "cprocessing.h"
#include "structs.h"

typedef struct Save {
	unsigned int isNewState;
	unsigned int lastLevelPlayed;
} Save;

typedef struct Settings
{
	unsigned int audio;
	float resWidth;
	float resHeight;
} Settings;

typedef struct Keybinds
{
	CP_KEY moveUp;
	CP_KEY moveLeft;
	CP_KEY moveDown;
	CP_KEY moveRight;
	CP_KEY undoMove;
	CP_KEY resetMap;
	CP_KEY exitGame;
} Keybinds;

typedef struct Config
{
	Save save;
	Settings settings;
	Keybinds keybinds;
} Config;

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
