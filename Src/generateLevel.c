#define _CRT_SECURE_NO_WARNINGS		// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#define _CRT_SECURE_NO_DEPRECATE	// Needed to use fopen & fscanf instead of fopen_s & fscanf_s
#include "structs.h"				// Needed for Grid and Customer Structs
#include "defines.h"				// Needed for define values
#include "levellogic.h"				// Needed to use Global Extern Variable 'level'
#include <stdio.h>					// Needed for parsing CSV file
#include <errno.h>					// Needed for error handling/checking of parsing CSV file

/* Parse CSV file to initialise grid array at the start of every stage/level */
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	printf("In setMap! \n");

	/* Initialise variables to 0 */
	int row = 0, col = 0, read = 0;

	/* Declare & initialise Local Customer Properties to 0 */
	int customerNumber = 0, customerPosX = 0, customerPosY = 0, customerDir = 0, customerRange = 0, customerActive = 0, customerIdle = 0, customerRandom = 0;

	// File Pointer of CSV File
	FILE* csv_file;

	// CSV File name & CSV Extension
	char csv_file_name[50] = "..\\..\\Extern\\level_mapper\\Seven11_Level_", level_char, csv_ext[5] = ".csv";

	// Convert Level Number to char to be used to append to csvfileName
	level_char = level + '0';

	/* Append Level Number & 'csv_ext' to 'csv_file_name' */
	strncat(csv_file_name, &level_char, 1);
	strcat(csv_file_name, csv_ext);

	// Open 'csv_file_name' in read mode
	csv_file = fopen(csv_file_name, "r");

	/* If file dows not exist & fails to open, print error and Exit program */
	if (NULL == csv_file) {
		printf("%s \n", &csv_file_name);
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(1);
	}
	do {

		/* Goes to Next Row of the Grid once Column reaches the end*/
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}

		/* Reads & Stores values from CSV File to Grid Struct & Local Customer Properties*/
		read = fscanf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			&grid[row][col].player, &grid[row][col].key, &grid[row][col].box, &grid[row][col].boarder, &grid[row][col].customer, 
			&customerNumber, &customerPosX, &customerPosY, &customerDir, &customerRange, &customerActive, &grid[row][col].shelf);
		
		/* If Customer Exists */
		if (customerNumber != 0 && grid[row][col].customer) {
			customerNumber--; // Decrement Customer Number value as element starts from 0

			/* Initialise values of properties of Customer Struct with values from CSV File that was first stored in Local Customer Properties*/
			customer[customerNumber].cusCol = customerPosX;
			customer[customerNumber].cusRow = customerPosY;
			customer[customerNumber].ogCusCol = customerPosX;
			customer[customerNumber].ogCusRow = customerPosY;
			customer[customerNumber].direction = customerDir;
			customer[customerNumber].range = customerRange;
			customer[customerNumber].isActive = customerActive;
			customer[customerNumber].isIdle = customerIdle;
			customer[customerNumber].isRandom = customerRandom;

			printf("Customer Number: %d, PosX: %d, PosY: %d, Direction: %d, Range: %d, Active: %d \n", customerNumber, customer[customerNumber].cusRow, customer[customerNumber].cusCol, customer[customerNumber].direction, customer[customerNumber].range, customer[customerNumber].isActive);
		}
		
		// Increments 'col' when the correct number of values were scanned
		if (read == 12) col++;
	} while (!feof(csv_file)); // While nbot End of File

	// Close CSV File
	fclose(csv_file);
}
