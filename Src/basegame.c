#include "cprocessing.h"
#include "structs.h"
#include "customer.h"
#include "level_generate.h"
#include "movement.h"
#include "utils.h"
#include "defines.h"
#include "settings.h"
#include "spritesheet.h"
#include "level_logic.h"
#include "level_transition.h"
#include "mechanics.h"
#include "mainmenu.h"
#include "level_overlay.h"
#include "options_draw.h"
#include <stdio.h>
#include <stdlib.h>

extern Config config;

Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Move moves[MOVE_MAX][SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];
Customer customer[CUSTOMER_MAX];

int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

float cellSize,cellAlign,sec,elapsedLock;

int totalObjs,isLocked,activatedCusX,activatedCusY,face,game_pause,clock;

float totalElapsedTime;

Button back, pause;

CP_Sound fail = NULL;

void base_Init(void) {

	/* Settings */
	CP_Settings_StrokeWeight(0.5f);
	// for clock settings
	CP_Settings_TextSize((float)config.settings.resolutionHeight*0.025f);
	
	/* Initializations */
	cellSize = (float)(CP_System_GetWindowHeight()/SOKOBAN_GRID_ROWS);
	cellAlign = (float)((CP_System_GetWindowWidth()-(int)cellSize*SOKOBAN_GRID_COLS)/2);
	face = 0;
	elapsedLock = 0;
	isLocked = 0;
	totalElapsedTime = 0;
	game_pause = 0;
	load_spritesheet(cellSize);
	setMap(grid, customer, path);				// Initialise Map
	totalObjs = getObjective(grid);				// Counts number of key objective to meet
	global_move = 1;							// Initialise move with 1 for rendering purposes*
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0, m = 0; col < SOKOBAN_GRID_COLS; col++) {
			moves[0][row][col].player = 0; //Initialise to 0 for rendering purposes
		}
	}

	/* SFX */
	fail = CP_Sound_Load("./Assets/Sound/Fail.wav");

	//setButton(&back, "./Assets/UI/Back.png", cellSize * 1.75f + cellAlign, cellSize * 1.75f, 2 * cellSize, 2 * cellSize, YES);
	//setButton(&pause, "./Assets/PAUSE.png", (float)(CP_System_GetWindowWidth() / 2.f), (float)(CP_System_GetWindowHeight() / 2.f), 248.f, 109.f, NO);
	//card_init();
	//wetsign_UM();
}

void base_Update(void) {
	int playerRow, playerCol, isCompleted = 0;
	float currentElapsedTime = CP_System_GetDt();
	
	if (CP_Input_KeyTriggered(KEY_P) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
		game_pause = !game_pause; 
	}
	///*Return to main menu*/
	//if (CP_Input_MouseClicked()) {
	//	if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
	//		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	//	}
	//}

	/*Read grid*/
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			/*Check if all objectives has been reached*/
			if (grid[row][col].key && grid[row][col].box)
				isCompleted++;

			/*Get position of player*/
			if (grid[row][col].player) {
				playerRow = row;
				playerCol = col;
			}

			if (customerLock(grid, customer)) {
				isLocked = 1;
			}
		}
	}

	if (!game_pause) {
		/*Clear Tint*/
		CP_Settings_NoTint();
		totalElapsedTime += currentElapsedTime;
		clock = duration - (int)totalElapsedTime;

		/* If all Objectives Met/Level Cleared, Move to Level Transition Screen */
		if (isCompleted == totalObjs) {
			next_level();
			CP_Engine_SetNextGameState(Level_Transition_Init, Level_Transition_Update, Level_Transition_Exit);
		}

		/* Lose Condition */
		if (clock == 0) {
			CP_Sound_Play(fail);
			game_pause = 1;
		}

		/*If player is stunlocked by customer, all inputs should be ignored.*/
		player_status(&isLocked); // UM
		if (isLocked) {
			// Moves the Customer to the player
			customerMoveToPlayer(playerRow, playerCol, grid, customer);
			/*Check if 3 seconds has passed*/
			if (elapsedLock <= lockTimer) {
				elapsedLock = elapsedLock + CP_System_GetDt();
				printf("Locked!\n");
			}
			else {
				/*Reset timer and turn customer inactive*/
				elapsedLock = 0;
				isLocked = 0;
				printf("Unlocked.\n");
			}
		}

		/*All inputs go here.*/
		else {
			/*Movement logic*/

			/*Check for input and get the direction of the input*/
			int dir = getDirection();

			/*Set direction that the player is facing.*/
			switch (dir) {
			case 1: // up
				face = 1;
				break;
			case 2: // left
				face = 2;
				break;
			case 3: // down
				face = 3;
				break;
			case 4: // right
				face = 4;
				break;
			}

			/*If there is movement.*/
			if (dir > 0) {
				saveMove(moves, grid);
				getCell(&playerRow, &playerCol, dir, grid);
			}

			/*Undo move.*/
			if (CP_Input_KeyTriggered(KEY_U) && global_move > 1) {
				undoMove(moves, grid);
				face = 0;
			}

			else if (CP_Input_KeyTriggered(KEY_R)) {
				resetMap(moves, grid, customer, path); //Resets grid to the initial values based on the CSV file
				face = 0;
			}
		}

		customerMovement(grid, path, customer);

		for (int i = 0; i < CUSTOMER_MAX; i++) {
			if (customer[i].isIdle) {
				customerIdle(i, customer);
			}
			else if (customer[i].isRandom) {
				randomCustomerMovement(grid, customer);
			}
		}
	}
	/*else {
		CP_Settings_Tint(DARKGRAY);
	}*/
	/*Rendering*/
	CP_Graphics_ClearBackground(BLUEGRAY);
	/*Rendering grid*/
	//CP_Graphics_ClearBackground(WHITE);
	
	// experimental
	//world_camera(cellSize,face,dir); // requires dir to be declared outside else loop
	
	/* Map Rendering */
	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
			Cell currCell = grid[row][col];

			float cellX = cellSize*(float)col+cellAlign;
			float cellY = cellSize*(float)row;
			
			draw_floor(cellX,cellY,cellSize);

			if (currCell.boarder || currCell.box || currCell.key || currCell.player || currCell.shelf || moves[global_move-1][row][col].player || 
				currCell.mecha || tele[0]==1 && (row==tele[1] && col==tele[2]) || (row==tele[3] && col==tele[4])) {
				if (currCell.boarder)
					draw_boarder(cellX,cellY,cellSize);

				else if (currCell.key && currCell.box)
					draw_key_in_box(cellX,cellY,cellSize);

				else if (currCell.key) 
					draw_key(cellX,cellY,cellSize);

				else if (tele[0]==1 && (row==tele[1] && col==tele[2]) || (row==tele[3] && col==tele[4]))
					draw_boarder(cellX,cellY,cellSize); // draw_tele();
				
				else if (currCell.mecha)
					draw_boarder(cellX,cellY,cellSize); // draw_mecha();

				if ( (currCell.player && (face==3||face==4||face==0)) ||  // renders initial position and when moving right/down
					(moves[global_move-1][row][col].player && (face==1||face==2)) )  // renders prev position if moving left/up				
					draw_player(&cellSize,&cellAlign, &playerRow, &playerCol, &face);  // bugfix: player doesn't render if turning left/up into obs if previous face was 3/4/0

				else if (currCell.box)
					draw_box(cellX,cellY,cellSize);

				else if (currCell.shelf)
					draw_boarder(cellX,cellY,cellSize);
			}
			if (currCell.customer) // currCell.customer holds current position and previous position
				for (int i = 0; i < CUSTOMER_MAX; i++) 
						draw_customer(&cellSize,&cellAlign,&customer[i].cusRow,&customer[i].cusCol,&customer[i].direction,&i);
		}
	}

	//customer[i].isIdle || (!(row==customer[i].cusRow) && col==customer[i].cusCol) ||	
						//(row==customer[i].cusRow && col==customer[i].cusCol) && !(customer[i].direction==1) && !(customer[i].direction==2)
	//(!(row==customer[i].cusRow) && col==customer[i].cusCol) || 
						//(row==customer[i].cusRow && col==customer[i].cusCol) && !(customer[i].direction==1) && !(customer[i].direction==2)
	//if (CP_Input_KeyTriggered(KEY_U)){
		//printf("Customer 0: R %d C %d \n",customer[0].cusRow,customer[0].cusCol);
		//printf("Customer 0: R %d C %d \n",customer[0].prevCusRow,customer[0].prevCusCol);
		//}
	if(game_pause) {
		CP_Settings_Tint(DARKGRAY);
		if (clock > 0) {
			overlay_pause();
			game_pause = unpause(game_pause);
		}
		else {
			overlay_game_over();
			game_pause = game_over(game_pause);
		}		
	}

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(BLACK);
	char buffer[20] = { 0 };
	sprintf_s(buffer, _countof(buffer), "Time left: %d", clock);
	CP_Font_DrawText(buffer, cellSize, cellSize);

}

void base_Exit(void) {
	CP_Sound_Free(&fail);
	free_sprite();
	CP_Settings_StrokeWeight(3.0f);
}
