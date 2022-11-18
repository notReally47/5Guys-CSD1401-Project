#pragma once
#include "cprocessing.h"
#include "structs.h"

typedef struct Save {
	int				isNewState;
	int				lastLevelPlayed;
} Save;

typedef struct Settings {
	int				titleBarHeight;
	int				audio;					// 0 - 100
	int				windowed;				// windowed: 1, fullscreen: 0
	unsigned int	resolutionWidth;		// 960, 1920
	unsigned int	resolutionHeight;		// 540, 1080
} Settings;

typedef struct UniqueCards {		// struct to be saved in config.dat
	int				negcards[7];	// holds {0,2,4,6,8,10,0} which is the relative position of the cards
	int				poscards[7];
	int				negdecksize;	// deck size decrements each selection
	int				posdecksize;
	unsigned int	flags;			// flag enabler for unique mechanics
	unsigned char	selectedflag;	// flag to check if a card is selected for that particular level
} UniqueCards;

typedef struct Config {
	Save			save;
	Settings		settings;
	UniqueCards		UM;
} Config;

typedef struct Customer {

	int				cusRow;					// Current Customer Row
	int				cusCol;					// Current Customer Column

	int				ogCusRow;				// Original Customer Row
	int				ogCusCol;				// Original Customer Column

	int				direction;				// Where Customer is facing
	int				range;					// Detection range, Current Default is 2
	int				isActive;				// Ability to stun player
	int				isIdle;					// Idle or not
	int				isRandom;				// Path is random or not
} Customer;

typedef struct Cell {
	int				player;					// Flag for Player, 1 is Player / 0 is Not Player
	int				key;					// Key Number, 0 if does not exist / Also Flag for Key, !0 is Key / 0 is not Key
	int				box;					// Box Number, 0 if does not exist / Also Flag for Box, !0 is Box / 0 is not Box
	int				tele;					// Teleporter Number, 0 if does not exist / Also Flag for Teleporter, !0 is Teleporter / 0 is not Teleporter
	int				boarder;				// Flag for Boarder, 1 is Boarder/ 0 is not Boarder
	int				customer;				// Customer Number, 0 if does not exist / Also Flag for Customer, !0 is Customer/ 0 is not Customer
	int				shelf;					// Flag for Shelf, 1 is Shelf / 0 is not Shelf
	int				mecha;					// Flag for Mechanics, 1 is Mechanics / 0 is not Mechanics
} Cell;

typedef struct Move {
	int				player;					// To save Player Position
	int				box;					// To save Box Poistions
} Move;

typedef struct Teleporter {
	int				teleporter_number;		// Teleporter Number
	int				posY;					// Teleporter Row
	int				posX;					// Teeleporter Column
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

typedef struct CardPosition {
	int				neg1;			// negative card position of left card
	int				neg2;			// negative card position of right card
	int				pos1;			// positive card position of left card
	int				pos2;			// positive card position of right card
} CardPosition;						// position is varied from 0 to 5 for the cards array
