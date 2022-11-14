#pragma once
#include "cprocessing.h"
#include "structs.h"

typedef struct Save {
	int				isNewState;
	int				lastLevelPlayed;
} Save;

typedef struct Settings {
	int				audio; /*0 - 100*/
	int				windowed; /*windowed: 1, fullscreen: 0*/
	unsigned int	resolutionWidth; /*960, 1920*/
	unsigned int	resolutionHeight; /*540, 1080*/
} Settings;

typedef struct Keybinds {
	CP_KEY			moveUp;
	CP_KEY			moveLeft;
	CP_KEY			moveDown;
	CP_KEY			moveRight;
	CP_KEY			undoMove;
	CP_KEY			resetMap;
	CP_KEY			exitGame;
} Keybinds;

typedef struct Config {
	Save			save;
	Settings		settings;
	Keybinds		keybinds;
} Config;

typedef struct Customer {
	/*Current position*/
	int				cusRow;
	int				cusCol;
	/*Original position*/
	int				ogCusRow;
	int				ogCusCol;

	int				direction;
	int				range; /*Detection range*/
	int				isActive; /*Ability to stun player.*/
	int				isIdle;
	int				isRandom;
} Customer;

typedef struct Cell {
	int				player;
	int				key;
	int				box;
	int				tele;
	int				boarder;
	int				customer;
	int				shelf;
	int				mecha;
} Cell;

typedef struct Move {
	int				player;
	int				box;
} Move;

typedef struct Teleporter {
	int				teleporter_number;
	int				posY;
	int				posX;
} Teleporter;

typedef struct rect {
	float			center_x;
	float			center_y;
	float			width;
	float			height;
} rect;

typedef struct Button {
	CP_Image		img;
	CP_Vector		position;
	float			btnWidth;
	float			btnHeight;
	int				tint;
} Button;

typedef struct DropDownList {
	unsigned int	actWidth;
	unsigned int	actHeight;
	int				windowed;
	int				selected;
	Button			button;
} DropDownList;

typedef struct GIF {
	CP_Image		spritesheet;
	CP_Vector		position;
	float			imgWidth;
	float			imgHeight;
	float			gifDimensions;
	int				numRows;
	int				numCols;
	int				currCol;
	int				imgIndex;
} GIF;

typedef struct UniqueCards {		// struct to be saved in config.dat
	int				negcards[7];	// holds {0,2,4,6,8,10,0} which is the relative position of the cards
	int				poscards[7];
	int				negdecksize;	// deck size decrements each selection
	int				posdecksize;
	unsigned int	flags;			// flag enabler for unique mechanics
	unsigned char	selectedflag;	// flag to check if a card is selected for that particular level
} UniqueCards;

typedef struct CardPosition {
	int				neg1;	// negative card position of left card
	int				neg2;	// negative card position of right card
	int				pos1;	// positive card position of left card
	int				pos2;	// positive card position of right card
} CardPosition;	// position is varied from 0 to 5 for the cards array
