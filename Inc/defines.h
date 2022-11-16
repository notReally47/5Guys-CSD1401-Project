#pragma once

//Colours
#define WHITE		CP_Color_Create(255, 255, 255, 255)
#define	BGLBLUE		CP_Color_Create(207, 239, 252, 255) // Background's Light Blue
#define	BGDBLUE		CP_Color_Create(183, 231, 250, 255) // Background's Dark Blue
#define	BGDBLUET	CP_Color_Create(183, 231, 250, 100) // Tint
#define PLYRGRN		CP_Color_Create(40, 177, 98, 255)	// Player's Dark Green
#define PLYRRED		CP_Color_Create(231, 76, 60, 255)	// Player's Light Red
#define GRAY		CP_Color_Create(150, 150, 150, 255)
#define DARKGRAY	CP_Color_Create(80, 80, 80, 100)	// Tint
#define BLACK		CP_Color_Create(0, 0, 0, 255)
#define ABITBLACK	CP_Color_Create(0, 0, 0, 210)
#define FAINTBLACK	CP_Color_Create(0, 0, 0, 200)
#define FADEBLACK	CP_Color_Create(0, 0, 0, 170)
#define FADERBLACK	CP_Color_Create(0, 0, 0, 140)
#define RED			CP_Color_Create(255, 0, 0, 200)
#define GREEN		CP_Color_Create(0, 255, 0, 255)
#define YELLOW		CP_Color_Create(255, 255, 0, 255)
#define BLUEGRAY	CP_Color_Create(136, 176, 215, 255)
#define BROWN		CP_Color_Create(165, 42, 42, 255)
#define VIOLET		CP_Color_Create(144, 70, 132, 255)
#define ORANGE		CP_Color_Create(226, 82, 47, 255)
#define CLEAR		CP_Color_Create(0, 0, 0, 0)

// Customer Colours
#define NEON_PINK	CP_Color_Create(255, 0, 127, 255)	// Moving Up
#define CARNATION	CP_Color_Create(255, 166, 201, 255)	// Moving Left
#define SALMON		CP_Color_Create(255, 134, 116, 255)	// Moving Down
#define COTTON		CP_Color_Create(255, 188, 217, 255)	// Moving Right

enum SOKOBAN {	
	SOKOBAN_IDLE,
	SOKOBAN_UP,
	SOKOBAN_LEFT,
	SOKOBAN_DOWN,
	SOKOBAN_RIGHT,
	SOKOBAN_FACE_UP,
	SOKOBAN_FACE_LEFT,
	SOKOBAN_FACE_DOWN,
	SOKOBAN_FACE_RIGHT,
	SOKOBAN_GRID_COLS = 41,
	SOKOBAN_GRID_ROWS = 31,
	TUTORIAL_GRID_COLS = 5,
	TUTORIAL_GRID_ROWS = 5,

	MOVE_MAX = 2000,
	TEXT_BUFFER = 255
};

enum CustomerStats {
	CUSTOMER_MAX = 10,
	CUSTOMER_SPEED = 10,
	CUSTOMER_TURN = 20,
	CUSTOMER_RANDOM_SPEED = 50
};

enum Options {
	CENTER,
	LEFT,
	RIGHT,
	CORNER,
	PADDING = 10,
	MAX_LENGTH = 20
};

typedef enum Logic {
	NO,
	YES
} Flag;

enum UniqueMechanics {
	MAX_DECK = 7,
	SETTINGS = 2,
	TELEPORTER_NUMBER = 8
};