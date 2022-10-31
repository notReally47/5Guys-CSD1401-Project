#define SOKOBAN_ROWS 31
#define SOKOBAN_COLS 41
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cell {
	int player;
	int key;
	int box;
	int boarder;
    int isCustomer;
    int CustomerNo;
    int CustomerPosX;
	int CustomerPosY;
	int CustomerDirection;
	int CustomerRange;
	int isActive;
	int shelf;
} Cell;

/* Initialise all values in all elements to 0 */
void emptyGrid (Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]){
	for (int row = 0; row < SOKOBAN_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_COLS; col++) {
			if(col == 0 || col == (SOKOBAN_COLS - 1) || row == 0 || row == (SOKOBAN_ROWS -1)) {
                grid[row][col].player = 0;
                grid[row][col].key = 0;
                grid[row][col].box = 0;
                grid[row][col].boarder = 1;
                grid[row][col].CustomerPosX = 0;
                grid[row][col].CustomerPosY = 0;
                grid[row][col].CustomerDirection = 0;
                grid[row][col].CustomerRange = 0;
                grid[row][col].isCustomer = 0;
                grid[row][col].isActive = 0;
                grid[row][col].shelf = 0;
            }else {
                grid[row][col].player = 0;
                grid[row][col].key = 0;
                grid[row][col].box = 0;
                grid[row][col].boarder = 0;
                grid[row][col].CustomerPosX = 0;
                grid[row][col].CustomerPosY = 0;
                grid[row][col].CustomerDirection = 0;
                grid[row][col].CustomerRange = 0;
                grid[row][col].isCustomer = 0;
                grid[row][col].isActive = 0;
                grid[row][col].shelf = 0;
            }
		}
	}
}

/* Set position of Player (row, column) */
void setPlayer(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int setRow = 0, setCol = 0, read = 0;
    printf("Please indicate the Player's position (row,col):\n");
    read = scanf("%d,%d", &setRow, &setCol);
    if(read == 2) {
        grid[setRow][setCol].player = 1;
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/* Set Position of Boxes & Keys */
void setBoxKey(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int setRow = 0, setCol = 0, counter = 0, addBox = 0, read = 0;
    printf("Please indicate the number of Boxes in this level:\n");
    read = scanf("%d", &addBox);
    if(read == 1) {
        counter = 0;
        if(addBox > 0) {
            while(counter < addBox) {
                printf("Please indicate Box position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &setRow, &setCol);
                if(read == 2) {
                    grid[setRow][setCol].box = 1;
                }
                else {
                    printf("Please indicate in the correct format! Exiting...\n");
                    exit(EXIT_FAILURE);
                }
                counter++;
            }
            counter = 0;
            while(counter < addBox) {
                printf("Please indicate Key position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &setRow, &setCol);
                if(read == 2) {
                    grid[setRow][setCol].key = 1;
                }
                else {
                    printf("Please indicate in the correct format! Exiting...\n");
                    exit(EXIT_FAILURE);
                }
                counter++;
            }
        }
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/* Sets Position of Customers (row)*/
void setCustomer(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int setRow = 0, setCol = 0, counter = 0, addCustomer = 0, setDirection = 0, read = 0, read1 = 0;
    printf("Please indicate the number of Customers in this level:\n");
    read = scanf("%d", &addCustomer);
    if(read == 1) {
        counter = 0;
        if(addCustomer > 0) {
            while(counter < addCustomer) {
                printf("Please indicate Customer position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &setRow, &setCol);
                printf("Please indicate Customer #%d's direction:\n", (counter + 1));
                read1 = scanf("%d", &setDirection);
                if(read == 2 && read1 == 1) {
                    grid[setRow][setCol].CustomerNo = counter + 1;
                    grid[setRow][setCol].CustomerPosX = setCol;
                    grid[setRow][setCol].CustomerPosY = setRow;
                    grid[setRow][setCol].CustomerDirection = setDirection;
                    grid[setRow][setCol].CustomerRange = 2;
                    grid[setRow][setCol].isCustomer = 1;
                    grid[setRow][setCol].isActive = 1;
                }
                else {
                    printf("Please indicate in the correct format! Exiting...\n");
                    exit(EXIT_FAILURE);
                }
                counter++;
            }
        }
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void setShelves(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int setRow = 0, setCol = 0, setRow1 = 0, setCol1 = 0, read = 0;
    char choice;
    printf("Are there Shelves? (Y/N) \n");
    scanf(" %c", &choice);
    if(choice == 'Y' || choice == 'y') {
        do{
            printf("Indicate range of Shelves (row start, column start, row end, column end):\n");
            read = scanf("%d,%d,%d,%d", &setRow, &setCol, &setRow1, &setCol1);
            if(read == 4) {
                if(setRow == setRow1) {
                    for(int cols = setCol; cols < setCol1 + 1; cols++) {
                        grid[setRow][cols].shelf = 1;
                    }
                }
                else if (setCol == setCol1) {
                    for(int rows = setRow; rows < setRow1 + 1; rows++) {
                            grid[rows][setCol].shelf = 1;
                    }
                }
                else {
                    printf("Please indicate Shelves in terms of rows/columns. \n");
                    exit(EXIT_FAILURE);
                }
            }
            printf("Any more Shelves? (Y/N)\n");
            read = scanf(" %c", &choice);
        }while(choice == 'Y' || choice == 'y');
    }


}

int main(void) {
    Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS];
    int read = 0, isPlayer = 0, isBox = 0, isKey = 0, isCustomer = 0, isBoarder = 0, isShelf = 0;
    char csvfileName[20] = "Seven11_Level_", arrayfileName[26] = "Seven11_Notes_Level_", level, csvExt[5] = ".csv", txtExt[5] = ".txt";

    printf("Please indicate the Level for this map:\n");
    read = scanf(" %c", &level);
    if(read == 1) {
        strncat(csvfileName, &level, 1);
        strcat(csvfileName, csvExt);

        strncat(arrayfileName, &level, 1);
        strcat(arrayfileName, txtExt);

        FILE* csv_file;
        FILE* array_reference;
        csv_file = fopen(csvfileName, "w");
        array_reference = fopen(arrayfileName, "w");

        emptyGrid(grid);
        setPlayer(grid);
        setBoxKey(grid);
        setCustomer(grid);
        setShelves(grid);

        printf("Level CSV generated!\n");

        for (int row = 0; row < SOKOBAN_ROWS; row++) {
            for (int col = 0; col < SOKOBAN_COLS; col++) {
                isPlayer = (grid[row][col].player) ? 1 : 0;
                isBox = (grid[row][col].box) ? 1 : 0;
                isKey = (grid[row][col].key) ? 1 : 0;
                isBoarder = (grid[row][col].boarder) ? 1 : 0;
                isCustomer = (grid[row][col].isCustomer) ? 1 : 0;
                isShelf = (grid[row][col].shelf) ? 1 : 0;

                fprintf(csv_file,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", grid[row][col].player, grid[row][col].key, grid[row][col].box, grid[row][col].boarder, grid[row][col].isCustomer,
                    grid[row][col].CustomerNo, grid[row][col].CustomerPosX, grid[row][col].CustomerPosY, grid[row][col].CustomerDirection, grid[row][col].CustomerRange, grid[row][col].isActive, grid[row][col].shelf);
                
                if(isPlayer) fprintf(array_reference, "[%d][%d] is Player\n", row, col);
                else if(isBox) fprintf(array_reference, "[%d][%d] is a Box\n", row, col);
                else if(isKey) fprintf(array_reference, "[%d][%d] is a Key\n", row, col);
                else if(isBoarder) fprintf(array_reference, "[%d][%d] is a Boarder\n", row, col);
                else if(isCustomer) fprintf(array_reference, "[%d][%d] is a Customer\n", row, col);
                else if(isShelf) fprintf(array_reference, "[%d][%d] is a Shelf\n", row, col);
                else fprintf(array_reference, "[%d][%d] is blank\n", row, col);
            }
        }

        fclose(csv_file);
        fclose(array_reference);
    }
    else {
        printf("Please key in only 1 number!\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
