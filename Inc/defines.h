#pragma once
#define SOKOBAN_GRID_COLS	50
#define SOKOBAN_GRID_ROWS	50
#define SOKOBAN_IDLE		0
#define SOKOBAN_UP			1
#define SOKOBAN_LEFT		2
#define SOKOBAN_DOWN		3
#define SOKOBAN_RIGHT		4
#define MOVE				2000

//Colours
#define WHITE		CP_Color_Create(255, 255, 255, 255)
#define GRAY		CP_Color_Create(150, 150, 150, 255)
#define BLACK		CP_Color_Create(0, 0, 0, 255)
#define RED			CP_Color_Create(255, 0, 0, 255)
#define GREEN		CP_Color_Create(0, 255, 0, 255)
#define YELLOW		CP_Color_Create(255, 255, 0, 255)
#define BLUEGRAY	CP_Color_Create(136, 176, 215, 255)
#define BROWN		CP_Color_Create(165, 42, 42, 255)
#define VIOLET		CP_Color_Create(144, 70, 132, 255)
#define ORANGE		CP_Color_Create(226, 82, 47, 255)

// Customer Colours
#define NEON_PINK	CP_Color_Create(255, 0, 127, 255)	// Moving Up
#define CARNATION	CP_Color_Create(255, 166, 201, 255)	// Moving Left
#define SALMON		CP_Color_Create(255, 134, 116, 255)	// Moving Down
#define COTTON		CP_Color_Create(255, 188, 217, 255)	// Moving Right

// Customer
#define CUSTOMER		10	// Maximum Number of Customers
#define CUSTOMER_SPEED	10	// The number of frames before the customer updates movement
#define CUSTOMER_TURN	20	// The number of frames before the customer updates rotation