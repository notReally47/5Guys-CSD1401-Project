#pragma once
#include "cprocessing.h"
#include "structs.h"

typedef struct Save {
	int isNewState;
	int lastLevelPlayed;
} Save;

typedef struct Settings {
	int audio;
	unsigned int resolutionWidth;
	unsigned int resolutionHeight;
} Settings;

typedef struct Keybinds {
	CP_KEY moveUp;
	CP_KEY moveLeft;
	CP_KEY moveDown;
	CP_KEY moveRight;
	CP_KEY undoMove;
	CP_KEY resetMap;
	CP_KEY exitGame;
} Keybinds;

typedef struct Config {
	Save save;
	Settings settings;
	Keybinds keybinds;
} Config;

typedef struct Customer {
	/*Current position*/
	int cusRow;
	int cusCol;
	/*Original position*/
	int ogCusRow;
	int ogCusCol;

	int direction;
	int range; /*Detection range*/
	int isActive; /*Ability to stun player.*/
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

typedef struct rect {
	float center_x;
	float center_y;
	float width;
	float height;
} rect;