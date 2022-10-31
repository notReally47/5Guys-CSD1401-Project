#define _CRT_SECURE_NO_WARNINGS //Added to use fopen & fscanf instead of fopen_s & fscanf_s
#include "cprocessing.h"
#include "structs.h"
#include "defines.h"
#include "utils.h"
#include "levellogic.h"
#include <stdio.h> //Added for parsing CSV file
#include <errno.h> //Added for error handling/checking of parsing CSV file

/*Parse CSV file to initialise grid array at the start of every stage/level*/
void setMap(Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS], Customer customer[CUSTOMER]) {
	printf("In setMap! \n");
	int row = 0, col = 0, read = 0, records = 0, customerNumber = 0, customerPosX = 0, customerPosY = 0, customerDir = 0, customerRange = 0, customerActive = 0;
	FILE* csvFile;
	char csvfileName[50] = "..\\..\\Extern\\level_mapper\\Seven11_Level_", level_char, csvExt[5] = ".csv";
	level_char = level + '0';
	strncat(csvfileName, &level_char, 1);
	strcat(csvfileName, csvExt);
	//char file[] = "..\\..\\Extern\\level_mapper\\Seven11_Level_1.csv";
	csvFile = fopen(csvfileName, "r");

	if (NULL == csvFile) {
		printf("%s \n", &csvfileName);
		printf("Error : errno='%s'.\n", strerror(errno));
		printf("File Opening Failed!\n");
		exit(1);
	}
	do {
		if (col == SOKOBAN_GRID_COLS) {
			row++;
			col = 0;
		}
		read = fscanf(csvFile, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &grid[row][col].player, &grid[row][col].key, &grid[row][col].box, &grid[row][col].boarder, &grid[row][col].customer, &customerNumber, &customerPosX, &customerPosY, &customerDir, &customerRange, &customerActive, &grid[row][col].shelf);
		if (customerNumber != 0 && grid[row][col].customer) {
			customerNumber--;
			customer[customerNumber].cusCol = customerPosX;
			customer[customerNumber].cusRow = customerPosY;
			customer[customerNumber].ogCusCol = customerPosX;
			customer[customerNumber].ogCusRow = customerPosY;
			customer[customerNumber].direction = customerDir;
			customer[customerNumber].range = customerRange;
			customer[customerNumber].isActive = customerActive;
			customer[customerNumber].isIdle = 0;
			customer[customerNumber].isRandom = 0;

			printf("Customer Number: %d, PosX: %d, PosY: %d, Direction: %d, Range: %d, Active: %d \n", customerNumber, customer[customerNumber].cusRow, customer[customerNumber].cusCol, customer[customerNumber].direction, customer[customerNumber].range, customer[customerNumber].isActive);
		}
	
		if (read == 12) col++;
	} while (!feof(csvFile));

	fclose(csvFile);
}
void levelSelect(int level) {

}