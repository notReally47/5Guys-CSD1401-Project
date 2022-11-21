#pragma once
/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: level_generate.c
Purpose	: Function to load level csv file to load game map
*/

#define _CRT_SECURE_NO_DEPRECATE	// Needed to use sprintf, fopen & fscanf instead of sprintf_s, fopen_s & fscanf_s
#include "utils.h"					// Needed for Global Extern duration
#include "structs.h"				// Needed for Grid, Customer & Teleporter Structs
#include "defines.h"				// Needed for define values
#include "level_logic.h"			// Needed to use Global Extern Variable 'level'
#include "movement.h"				// Needed for Global Extern move_limit
#include <stdio.h>					// Needed for parsing CSV file
#include <stdlib.h>					// Needed for exit() function
#include <errno.h>					// Needed for error handling/checking of parsing CSV file

int original_box_count = 0;

/* Parse CSV file to initialise grid array at the start of every stage/level */
void set_map(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER_MAX], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Teleporter teleporters[TELEPORTER_NUMBER]) {

	int row = 0, col = 0, read = 0, line = 0, teleporter_index = 0,								// row & col for grid array, teleporter_index for teleporter array, line for lines in CSV File
		customer_number = 0, customer_posX = 0, customer_posY = 0, customer_direction = 0,		// Local Customer variables to 0
		customer_range = 0, customer_active = 0, customer_idle = 0, customer_random = 0;		// Local Customer variables to 0

	original_box_count = 0;

	/* For-Loop to clear out the Map First (Prevent carry-over from previous levels) */
	for (int i = 0; i < SOKOBAN_GRID_ROWS; i++) {
		for (int j = 0; j < SOKOBAN_GRID_COLS; j++) {
			grid[i][j].player = 0;
			grid[i][j].key = 0;
			grid[i][j].box = 0;
			grid[i][j].tele = 0;
			grid[i][j].boarder = 0;
			grid[i][j].shelf = 0;
			grid[i][j].mecha = 0;
			grid[i][j].customer = 0;
			path[i][j] = 0;
		}
	}
	/* For-Loop to clear all customers first (Prevent carry-over from previous levels) */
	for (int i = 0; i < CUSTOMER_MAX; i++) {
		customer[i].cusCol = 0;
		customer[i].cusRow = 0;
		customer[i].ogCusCol = 0;
		customer[i].ogCusRow = 0;
		customer[i].direction = 0;
		customer[i].range = 0;
		customer[i].isActive = 0;
		customer[i].isIdle = 0;
		customer[i].isRandom = 0;
	}

	/* For-Loop to clear all Teleporter first (Prevent carry-over from previous levels) */
	for (int i = 0; i < TELEPORTER_NUMBER; i++) {
		teleporters[i].teleporter_number = 0;
		teleporters[i].posX = 0;
		teleporters[i].posY = 0;
	}

	FILE* csv_file;																				// File Pointer of CSV File
	char csv_file_name[64] = "./Assets/level_mapper/level_files/Seven11_Level_";				// Start of the level file name
	char level_char[3], csv_ext[5] = ".csv";													// Level Number & .csv Extension
	sprintf(level_char, "%d", global_level);													// Convert Level Number to char to be used to append to csv_file_name
	strcat(csv_file_name, level_char);															// Append Level Number
	strcat(csv_file_name, csv_ext);																// Append .csv extension
	csv_file = fopen(csv_file_name, "r");														// Open csv_file_name in read mode

	/* If file does not exist & fails to open, print error and Exit program */
	if (NULL == csv_file) {
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(EXIT_FAILURE);
	}

	/* Loop Each Line of the CSV File */
	do {
		/* Goes to Next Row of the Grid once Column reaches the end */
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}
		if (line == 0) {																		// First Line of the CSV file is the Level Duration
			read = fscanf(csv_file, "%d", &duration);											// Scan and store to global variable 'duration'
			if (read == 1) line++;																// Increment line so that function will not read for duration again
		}
		else if (line == 1) {																	// Second Line of the CSV file is the Level Duration
			read = fscanf(csv_file, "%d", &move_limit);											// Scan and store to global variable 'move_limit'
			if (read == 1) line++;																// Increment line so that function will not read for duration again
		}
		else {
			/* Reads & Stores values from CSV File to Grid Struct & Local Customer Properties */
			read = fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				&grid[row][col].player, &grid[row][col].key, &grid[row][col].box, &grid[row][col].tele, &grid[row][col].boarder, &grid[row][col].shelf,
				&grid[row][col].mecha, &grid[row][col].customer, &customer_number, &customer_posX, &customer_posY, &customer_direction, &customer_range,
				&customer_active, &customer_idle, &customer_random, &path[row][col]);

			/* If Customer Exists */
			if (customer_number != 0 && grid[row][col].customer) {								// If Customer is not 0 (Exists)
				customer_number--;																// Decrement Customer Number value as element starts from 0

				/* Initialise values of properties of Customer Struct with values from CSV File that was first stored in Local Customer Properties */
				customer[customer_number].cusCol = customer_posX;
				customer[customer_number].cusRow = customer_posY;
				customer[customer_number].ogCusCol = customer_posX;
				customer[customer_number].ogCusRow = customer_posY;
				customer[customer_number].direction = customer_direction;
				customer[customer_number].range = customer_range;
				customer[customer_number].isActive = customer_active;
				customer[customer_number].isIdle = customer_idle;
				customer[customer_number].isRandom = customer_random;
			}

			/* If Teleporter Exists, Initialise Teleporter Struct Property Values, with Teleporter Number ,Row and Column */
			if (grid[row][col].tele != 0) {
				teleporter_index = grid[row][col].tele - 1;										// Decrement grid[row][col].tele value as element starts from 0
				teleporters[teleporter_index].teleporter_number = grid[row][col].tele;
				teleporters[teleporter_index].posX = col;
				teleporters[teleporter_index].posY = row;
			}

			if (grid[row][col].box)
				original_box_count++;															// COunts original number of boxes set for each level

			// Increments 'col' & 'line' when the correct number of values were scanned
			if (read == 17) {
				col++;
				line++;
			}
		}
		
	} while (!feof(csv_file)); // While not End of File

	// Close CSV File
	fclose(csv_file);

}
