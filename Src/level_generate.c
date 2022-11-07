#define _CRT_SECURE_NO_WARNINGS		// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#define _CRT_SECURE_NO_DEPRECATE	// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#include "utils.h"					// Needed for Global Extern duration
#include "structs.h"				// Needed for Grid and Customer Structs
#include "defines.h"				// Needed for define values
#include "level_logic.h"			// Needed to use Global Extern Variable 'level'
#include <stdio.h>					// Needed for parsing CSV file
#include <stdlib.h>					// Needed for exit() function
#include <errno.h>					// Needed for error handling/checking of parsing CSV file

/* Parse CSV file to initialise grid array at the start of every stage/level */
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER], int path[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS]) {
	
	printf("In setMap! \n");

	/* Initialise variables to 0 */
	int row = 0, col = 0, read = 0, line = 0;

	/* Declare & initialise Local Customer Properties to 0 */
	int customer_number = 0, customer_posX = 0, customer_posY = 0, customer_direction = 0, customer_range = 0, customer_active = 0, customer_idle = 0, customer_random = 0;

	/* For-Loop to clear all customers first (Prevent carry-over from previous levels) */
	for (int i = 0; i < CUSTOMER; i++) {
		customer[i].cusCol = customer_posX;
		customer[i].cusRow = customer_posY;
		customer[i].ogCusCol = customer_posX;
		customer[i].ogCusRow = customer_posY;
		customer[i].direction = customer_direction;
		customer[i].range = customer_range;
		customer[i].isActive = customer_active;
		customer[i].isIdle = customer_idle;
		customer[i].isRandom = customer_random;
	}

	// File Pointer of CSV File
	FILE* csv_file;

	// CSV File name & CSV Extension
	char csv_file_name[63] = "..\\..\\Assets\\level_mapper\\level_files\\Seven11_Level_";
	char level_char, csv_ext[5] = ".csv";

	// Convert Level Number to char to be used to append to csvfileName
	level_char = global_level + '0';

	/* Append Level Number & 'csv_ext' to 'csv_file_name' */
	strncat(csv_file_name, &level_char, 1);
	strcat(csv_file_name, csv_ext);

	// Open 'csv_file_name' in read mode
	csv_file = fopen(csv_file_name, "r");

	/* If file does not exist & fails to open, print error and Exit program */
	if (NULL == csv_file) {
		printf("%s \n", &csv_file_name);
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(EXIT_FAILURE);
	}

	do {

		/* Goes to Next Row of the Grid once Column reaches the end */
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}
		if (line == 0) {
			read = fscanf(csv_file, "%d", &duration);
			if (read == 1) line++;
		}
		else {
			/* Reads & Stores values from CSV File to Grid Struct & Local Customer Properties */
			read = fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				&grid[row][col].player, &grid[row][col].key, &grid[row][col].box, &grid[row][col].boarder, &grid[row][col].shelf, &grid[row][col].mecha,
				&grid[row][col].customer, &customer_number, &customer_posX, &customer_posY, &customer_direction, &customer_range,
				&customer_active, &customer_idle, &customer_random, &path[row][col]);

			/* If Customer Exists */
			if (customer_number != 0 && grid[row][col].customer) {
				customer_number--; // Decrement Customer Number value as element starts from 0

				/* Initialise values of properties of Customer Struct with values from CSV File that was first stored in Local Customer Properties*/
				customer[customer_number].cusCol = customer_posX;
				customer[customer_number].cusRow = customer_posY;
				customer[customer_number].ogCusCol = customer_posX;
				customer[customer_number].ogCusRow = customer_posY;
				customer[customer_number].direction = customer_direction;
				customer[customer_number].range = customer_range;
				customer[customer_number].isActive = customer_active;
				customer[customer_number].isIdle = customer_idle;
				customer[customer_number].isRandom = customer_random;

				printf("Customer Number: %d, PosX: %d, PosY: %d, Direction: %d, Range: %d, Active: %d \n",
					customer_number, customer[customer_number].cusRow, customer[customer_number].cusCol,
					customer[customer_number].direction, customer[customer_number].range, customer[customer_number].isActive);
			}

			// Increments 'col' when the correct number of values were scanned
			if (read == 16) {
				col++;
				line++;
			}
		}
		
	} while (!feof(csv_file)); // While not End of File

	// Close CSV File
	fclose(csv_file);
}
