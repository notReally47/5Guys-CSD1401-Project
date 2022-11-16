#define SOKOBAN_ROWS 31
#define SOKOBAN_COLS 41
#include <stdio.h>          // Needed for file I/O, r/w and printf/scanf
#include <stdlib.h>         // Needed for exit() function
#include <string.h>         // Needed for string concatenation

typedef struct Cell {
    // For Grid Struct in the Project
	int player;
	int key;
	int box;
	int border;
    int shelf;
    int mecha;
    int tele;
    int path;

    // For Customer Struct in the Project
    int is_customer;
    int customer_no;
    int customer_posX;
	int customer_posY;
	int customer_direction;
	int customer_range;
	int is_active;
    int customer_idle;
    int customer_random;
} Cell;

/* Initialise all values in all elements to 0 & 4 Walls/Boarders to 1 */
void empty_grid (Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]){
	for (int row = 0; row < SOKOBAN_ROWS; row++) {
		for (int col = 0; col < SOKOBAN_COLS; col++) {
			if(col == 0 || col == (SOKOBAN_COLS - 1) || row == 0 || row == (SOKOBAN_ROWS -1)) {
                grid[row][col].player = 0;
                grid[row][col].key = 0;
                grid[row][col].box = 0;
                grid[row][col].border = 1;
                grid[row][col].shelf = 0;
                grid[row][col].tele = 0;
                grid[row][col].mecha = 0;
                grid[row][col].path = 0;
                grid[row][col].customer_posX = 0;
                grid[row][col].customer_posY = 0;
                grid[row][col].customer_direction = 0;
                grid[row][col].customer_range = 0;
                grid[row][col].is_customer = 0;
                grid[row][col].is_active = 0;  
                grid[row][col].customer_idle = 0;
                grid[row][col].customer_random = 0;
                grid[row][col].customer_no = 0;
            }else {
                grid[row][col].player = 0;
                grid[row][col].key = 0;
                grid[row][col].box = 0;
                grid[row][col].border = 0;
                grid[row][col].shelf = 0;
                grid[row][col].tele = 0;
                grid[row][col].mecha = 0;
                grid[row][col].path = 0;
                grid[row][col].customer_posX = 0;
                grid[row][col].customer_posY = 0;
                grid[row][col].customer_direction = 0;
                grid[row][col].customer_range = 0;
                grid[row][col].is_customer = 0;
                grid[row][col].is_active = 0;
                grid[row][col].customer_idle = 0;
                grid[row][col].customer_random = 0;
                grid[row][col].customer_no = 0;
            }
		}
	}
}

/* Set Duration of Level */
int set_duration() {
    int duration = 0, read = 0;
    printf("Please indicate the Duration for this level (s):\n");
    read = scanf("%d", &duration);
    if(read == 1) {
        return duration;
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/* Set Move Limit of Level */
int set_move_limit() {
    int move_limit = 0, read = 0;
    printf("Please indicate the Move Count Limit for this level:\n");
    read = scanf("%d", &move_limit);
    if(read == 1) {
        return move_limit;
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/* Set position of Player (row, column) */
void set_player(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int set_row = 0, set_col = 0, read = 0;
    printf("Please indicate the Player's position (row,col):\n");
    read = scanf("%d,%d", &set_row, &set_col);
    if(read == 2) {
        grid[set_row][set_col].player = 1;
    }
    else {
        printf("Please indicate in the correct format! Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/* Set Position of Boxes & Keys */
void set_box_key(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int set_row = 0, set_col = 0, counter = 0, add_box = 0, read = 0;
    printf("Please indicate the number of Boxes in this level:\n");
    read = scanf("%d", &add_box);
    if(read == 1) {
        counter = 0;
        if(add_box > 0) {
            while(counter < add_box) {
                printf("Please indicate Box position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &set_row, &set_col);
                if(read == 2) {
                    grid[set_row][set_col].box = counter + 1;
                }
                else {
                    printf("Please indicate in the correct format! Exiting...\n");
                    exit(EXIT_FAILURE);
                }
                counter++;
            }
            counter = 0;
            while(counter < add_box) {
                printf("Please indicate Key position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &set_row, &set_col);
                if(read == 2) {
                    grid[set_row][set_col].key = counter + 1;
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

/* Set position of Teleporters (row, column) */
void set_teleporter(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int set_row = 0, set_col = 0, read = 0, add_tele = 0, counter = 0;
    char choice = '\0';
    printf("Does this level have Teleporters (Y/N):\n");
    scanf(" %c", &choice);
    if(choice == 'Y' || choice == 'y') {
        printf("Please indicate the sets of number of Teleporters: \n");
        read = scanf("%d", &add_tele);
        add_tele *= 2;
        if(read == 1) {
            counter = 0;
            if(add_tele > 0) {
                while(counter < add_tele) {
                    printf("Please indicate Teleporter position #%d (row,col):\n", (counter + 1));
                    read = scanf("%d,%d", &set_row, &set_col);
                    if(read == 2) {
                        grid[set_row][set_col].tele = counter + 1;
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
}

/* Sets Position of Customers (row, column) & Direction (1/2/3/4) */
void set_customer(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int set_row = 0, set_col = 0, counter = 0, add_customer = 0, set_direction = 0, set_idle = 0, set_random = 0, read = 0, read1 = 0, waypoint = 0;
    char choice = '\0';
    printf("Please indicate the number of Customers in this level:\n");
    read = scanf("%d", &add_customer);
    if(read == 1) {
        counter = 0;
        if(add_customer > 0) {
            while(counter < add_customer) {
                printf("Please indicate Customer position #%d (row,col):\n", (counter + 1));
                read = scanf("%d,%d", &set_row, &set_col);
                printf("Please indicate Customer #%d's direction:\n", (counter + 1));
                read1 = scanf("%d", &set_direction);
                printf("Is Customer Idle? (Y/N)\n");
                scanf(" %c", &choice);
                set_idle = (choice == 'Y' || choice == 'y') ? 1 : 0;
                printf("Is Customer Random? (Y/N)\n");
                scanf(" %c", &choice);
                set_random = (choice == 'Y' || choice == 'y') ? 1 : 0;
                if(read == 2 && read1 == 1) {
                    grid[set_row][set_col].customer_no = counter + 1;
                    grid[set_row][set_col].customer_posX = set_col;
                    grid[set_row][set_col].customer_posY = set_row;
                    grid[set_row][set_col].customer_direction = set_direction;
                    grid[set_row][set_col].customer_range = 2;
                    grid[set_row][set_col].is_customer = 1;
                    grid[set_row][set_col].is_active = 1;
                    grid[set_row][set_col].customer_idle = set_idle;
                    grid[set_row][set_col].customer_random = set_random;

                    if(!grid[set_row][set_col].customer_idle) {
                        printf("For Non-Idle Customers, are there Waypoints? (Y/N)\n");
                        scanf(" %c", &choice);
                        if(choice == 'Y' || choice == 'y') {
                            do {
                                printf("Indicate Waypoint position & Direction(row,col,direction):\n");
                                read = scanf("%d,%d,%d", &set_row, &set_col, &set_direction);
                                if(read == 3) {
                                    grid[set_row][set_col].path = set_direction;
                                }
                                else {
                                    printf("Please indicate in the correct format! Exiting...\n");
                                    exit(EXIT_FAILURE);
                                }
                                printf("Any more Waypoints? (Y/N)\n");
                                scanf(" %c", &choice);
                            }while(choice == 'Y' || choice == 'y');
                        }
                    }
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

/* Set Shelves in rows/columns (row start, column start, row end, column end)*/
void set_shelves(Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS]) {
    int set_row_start = 0, set_col_start = 0, set_row_end = 0, set_col_end = 0, read = 0;
    char choice;
    printf("Are there Shelves? (Y/N) \n");
    scanf(" %c", &choice);
    if(choice == 'Y' || choice == 'y') {
        do{
            printf("Indicate range of Shelves (row start, column start, row end, column end):\n");
            read = scanf("%d,%d,%d,%d", &set_row_start, &set_col_start, &set_row_end, &set_col_end);
            if(read == 4) {
                if(set_row_start == set_row_end) {
                    for(int cols = set_col_start; cols < set_col_end + 1; cols++) {
                        grid[set_row_start][cols].shelf = 1;
                    }
                }
                else if (set_col_start == set_col_end) {
                    for(int rows = set_row_start; rows < set_row_end + 1; rows++) {
                            grid[rows][set_col_start].shelf = 1;
                    }
                }
                else {
                    printf("Please indicate Shelves in terms of rows/columns. \n");
                    exit(EXIT_FAILURE);
                }
            }
            printf("Any more Shelves? (Y/N)\n");
            read = scanf(" %c", &choice);
        }while(choice == 'Y' || choice == 'y'); // Keeps looping if Adding more Shelves
    }
}

int main(void) {
    Cell grid[SOKOBAN_ROWS][SOKOBAN_COLS];

    /* Declare & Initialise read to 0 & boolean checks of different properties to 0 */
    int read = 0, is_player = 0, is_box = 0, is_key = 0, is_customer = 0, is_border = 0, is_shelf = 0, is_tele = 0, is_waypoint = 0, duration = 0, move_limit = 0, level;

    /* Initialise file name without the level number and extension name */
    char csv_file_name[33] = "level_files/Seven11_Level_", reference_file_name[39] = "level_files/Seven11_Notes_Level_", level_str[3], 
        csv_ext[5] = ".csv", txt_ext[5] = ".txt", csv_bak_file[38] = "level_files/Seven11_Level_", bak_file[5] = "_bak";

    // Get level number
    printf("Please indicate the Level for this map:\n");
    read = scanf(" %d", &level);
    if(read == 1) {

        sprintf(level_str, "%d", level);

        /* Appends 'csv_file_name' with level number and csv extension */
        strcat(csv_file_name, level_str);
        strcat(csv_file_name, csv_ext);
        
        /* Appends the backup file similarly but with '_bak_*/
        strcat(csv_bak_file, level_str);
        strcat(csv_bak_file, bak_file);
        strcat(csv_bak_file, csv_ext);

        /* Appends 'reference_file_name' with level number and txt extension */
        strcat(reference_file_name, level_str);
        strcat(reference_file_name, txt_ext);

        // File pointers for csv_file_name & reference_file_name
        FILE* csv_file;
        FILE* csv_bak;
        FILE* array_reference;

        // Opens 'csv_file_name' & 'reference_file_name' in write mode
        csv_file = fopen(csv_file_name, "w");
        csv_bak = fopen(csv_bak_file, "w");
        array_reference = fopen(reference_file_name, "w");

        // Sets all settings
        duration = set_duration();
        move_limit = set_move_limit();
        empty_grid(grid);
        set_player(grid);
        set_box_key(grid);
        set_teleporter(grid);
        set_customer(grid);
        set_shelves(grid);

        printf("Level CSV generated!\n");

        fprintf(csv_file, "%d\n", duration);
        fprintf(csv_bak, "%d\n", duration);
        fprintf(array_reference, "Level Duration is %d\n", duration);
        fprintf(csv_file, "%d\n", move_limit);
        fprintf(csv_bak, "%d\n", move_limit);
        fprintf(array_reference, "Level Move Limit is %d\n", move_limit);
        
        // Prints all the values for the CSV File & Reference File
        for (int row = 0; row < SOKOBAN_ROWS; row++) {
            for (int col = 0; col < SOKOBAN_COLS; col++) {

                fprintf(csv_file,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", grid[row][col].player, grid[row][col].key, grid[row][col].box, grid[row][col].tele, grid[row][col].border, 
                    grid[row][col].shelf, grid[row][col].mecha, grid[row][col].is_customer, grid[row][col].customer_no, grid[row][col].customer_posX, grid[row][col].customer_posY,
                    grid[row][col].customer_direction, grid[row][col].customer_range, grid[row][col].is_active, grid[row][col].customer_idle, grid[row][col].customer_random, grid[row][col].path);

                fprintf(csv_bak,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", grid[row][col].player, grid[row][col].key, grid[row][col].box, grid[row][col].tele, grid[row][col].border, 
                    grid[row][col].shelf, grid[row][col].mecha, grid[row][col].is_customer, grid[row][col].customer_no, grid[row][col].customer_posX, grid[row][col].customer_posY,
                    grid[row][col].customer_direction, grid[row][col].customer_range, grid[row][col].is_active, grid[row][col].customer_idle, grid[row][col].customer_random, grid[row][col].path);
                
                is_player = (grid[row][col].player) ? 1 : 0;
                is_box = (grid[row][col].box) ? 1 : 0;
                is_key = (grid[row][col].key) ? 1 : 0;
                is_border = (grid[row][col].border) ? 1 : 0;
                is_tele = (grid[row][col].tele) ? 1 : 0;
                is_customer = (grid[row][col].is_customer) ? 1 : 0;
                is_shelf = (grid[row][col].shelf) ? 1 : 0;
                is_waypoint = (grid[row][col].path != 0) ? 1 : 0;

                if(is_player) fprintf(array_reference, "[%d][%d] is Player\n", row, col);
                else if(is_box) fprintf(array_reference, "[%d][%d] is a Box\n", row, col);
                else if(is_key) fprintf(array_reference, "[%d][%d] is a Key\n", row, col);
                else if(is_border) fprintf(array_reference, "[%d][%d] is a Border\n", row, col);
                else if(is_tele) fprintf(array_reference, "[%d][%d] is a Teleporter\n", row, col);
                else if(is_customer) fprintf(array_reference, "[%d][%d] is a Customer\n", row, col);
                else if(is_customer && is_waypoint) fprintf(array_reference, "[%d][%d] is a Customer & a Waypoint\n", row, col);
                else if(is_shelf) fprintf(array_reference, "[%d][%d] is a Shelf\n", row, col);
                else if(is_waypoint) fprintf(array_reference, "[%d][%d] is a Waypoint\n", row, col);
                else fprintf(array_reference, "[%d][%d] is Blank\n", row, col);
            }
        }

        // Close all file
        fclose(csv_file);
        fclose(csv_bak);
        fclose(array_reference);
    }
    else {
        printf("Please key in only 1 number!\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
