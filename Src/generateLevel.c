#define _CRT_SECURE_NO_WARNINGS		// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#define _CRT_SECURE_NO_DEPRECATE	// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#include "structs.h"				// Needed for Grid and Customer Structs
#include "defines.h"				// Needed for define values
#include "levellogic.h"				// Needed to use Global Extern Variable 'level'
#include <stdio.h>					// Needed for parsing CSV file
#include <stdlib.h>					// Needed for exit() function
#include <errno.h>					// Needed for error handling/checking of parsing CSV file

/* Parse CSV file to initialise grid array at the start of every stage/level */
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	printf("In setMap! \n");
	//errno_t err;
	/* Initialise variables to 0 */
	int row = 0, col = 0, read = 0;

	/* Declare & initialise Local Customer Properties to 0 */
	int customer_number = 0, customer_posX = 0, customer_posY = 0, customer_direction = 0, customer_range = 0, customer_active = 0, customer_idle = 0, customer_random = 0;

	// File Pointer of CSV File
	FILE* csv_file;

	// CSV File name & CSV Extension
	char csv_file_name[63] = "..\\..\\Assets\\level_mapper\\level_files\\Seven11_Level_";
	char level_char, csv_ext[5] = ".csv";

	// Convert Level Number to char to be used to append to csvfileName
	level_char = level + '0';

	/* Append Level Number & 'csv_ext' to 'csv_file_name' */
	strncat(csv_file_name, &level_char, 1);
	strcat(csv_file_name, csv_ext);

	// Open 'csv_file_name' in read mode
	csv_file = fopen(csv_file_name, "r");
	//err = fopen_s(&csv_file, csv_file_name, "r");

	/* If file does not exist & fails to open, print error and Exit program */
	if (NULL == csv_file) {
		printf("%s \n", &csv_file_name);
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(EXIT_FAILURE);
	}

	for (int row = 0; row < SOKOBAN_GRID_ROWS; row++) {			// TEMPORARY
		for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {		// TO BE INTEGRATED
			grid[row][col].mecha = 0;							// INTO CSV
		}
	}
	do {

		/* Goes to Next Row of the Grid once Column reaches the end */
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}

		/* Reads & Stores values from CSV File to Grid Struct & Local Customer Properties */
		read = fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			&grid[row][col].player, &grid[row][col].key, &grid[row][col].box, &grid[row][col].boarder, &grid[row][col].shelf, &grid[row][col].customer,
			 &customer_number, &customer_posX, &customer_posY, &customer_direction, &customer_range, &customer_active, &customer_idle, &customer_random);
		
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
		if (read == 14) col++;
	} while (!feof(csv_file)); // While not End of File

	// Close CSV File
	fclose(csv_file);
}
