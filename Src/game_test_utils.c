#include "cprocessing.h"
#include<stdio.h>
#include<math.h>

#define BOX_COLS 10
#define BOX_ROWS 10

#define WHITE CP_Color_Create(255, 255, 255, 255)
#define GRAY CP_Color_Create(150, 150, 150, 255)
#define BLACK CP_Color_Create(0, 0, 0, 255)
#define RED CP_Color_Create(255, 0, 0, 255)
#define GREEN CP_Color_Create(0, 255, 0, 255)
#define YELLOW CP_Color_Create(255, 255, 0, 255)
#define BLUEGRAY CP_Color_Create(136, 176, 215, 255)
#define BROWN CP_Color_Create(165, 42, 42, 255)
#define VIOLET CP_Color_Create(144, 70, 132, 255)

struct TILES {
    int has_player, has_box, is_objective, is_wall;
};

void draw_assets(struct TILES tile, int rows, int cols, float box_row, float box_col, float tile_width, float tile_height) {
    (tile.has_player == 1) ? CP_Settings_Fill(GREEN) : 
        ((tile.has_box == 1 && tile.is_objective == 1) ? CP_Settings_Fill(VIOLET) : 
            ((tile.has_box == 1) ? CP_Settings_Fill(BLUEGRAY) : 
                (((tile.is_objective == 1) ? CP_Settings_Fill(YELLOW) :
                    (((tile.is_wall == 1) ? CP_Settings_Fill(BROWN) : CP_Settings_Fill(GRAY)))))));
    CP_Graphics_DrawRect(box_col, box_row, tile_width, tile_height);
}

void move_up(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]) {
    if (tile[rows][cols].has_player == 1) {
        printf("Move Up Row: %d \n", rows);
        if ((rows - 2) >= 0) {
            if (tile[rows - 1][cols].has_box == 1 && tile[rows - 2][cols].has_box == 0 && tile[rows - 2][cols].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows - 1][cols].has_box = 0;
                tile[rows - 1][cols].has_player = 1;
                tile[rows - 2][cols].has_box = 1;
            }
            else if (rows > 0 && tile[rows - 1][cols].has_box == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows - 1][cols].has_player = 1;
            }
        }
        else {
            if (tile[rows][cols].has_player == 1 && rows > 0 && tile[rows - 1][cols].has_box == 0 && tile[rows - 1][cols].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows - 1][cols].has_player = 1;
            }
        }
    }
}

void move_down(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]) {
    if (tile[rows][cols].has_player == 1) {
        printf("Move Down Row: %d \n", rows);
        if ((rows + 2) <= (BOX_ROWS - 1)) {
            if (tile[rows + 1][cols].has_box == 1 && tile[rows + 2][cols].has_box == 0 && tile[rows + 2][cols].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows + 1][cols].has_box = 0;
                tile[rows + 1][cols].has_player = 1;
                tile[rows + 2][cols].has_box = 1;
            }
            else if (rows < (BOX_ROWS - 1) && tile[rows + 1][cols].has_box == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows + 1][cols].has_player = 1;
            }
        }
        else {
            if (tile[rows][cols].has_player == 1 && rows < (BOX_ROWS - 1) && tile[rows + 1][cols].has_box == 0 && tile[rows  + 1][cols].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows + 1][cols].has_player = 1;
            }
        }
    }
}

void move_left(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]) {
    if (tile[rows][cols].has_player == 1) {
        printf("Move Left Column: %d \n", cols);
        if ((cols - 2) >= 0) {
            if (tile[rows][cols - 1].has_box == 1 && tile[rows][cols - 2].has_box == 0 && tile[rows][cols - 2].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols - 1].has_box = 0;
                tile[rows][cols - 1].has_player = 1;
                tile[rows][cols - 2].has_box = 1;
            }
            else if (cols > 0 && tile[rows][cols - 1].has_box == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols - 1].has_player = 1;
            }
        }
        else {
            if (tile[rows][cols].has_player == 1 && cols > 0 && tile[rows][cols - 1].has_box == 0 && tile[rows][cols - 1].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols - 1].has_player = 1;
            }
        }
    }
}

void move_right(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]) {
    if (tile[rows][cols].has_player == 1) {
        printf("Move Right Column: %d \n", cols);
        if ((cols + 2) <= (BOX_COLS - 1)) {
            if (tile[rows][cols + 1].has_box == 1 && tile[rows][cols + 2].has_box == 0 && tile[rows][cols + 2].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols + 1].has_box = 0;
                tile[rows][cols + 1].has_player = 1;
                tile[rows][cols + 2].has_box = 1;
            }
            else if (cols < (BOX_COLS - 1) && tile[rows][cols + 1].has_box == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols + 1].has_player = 1;
            }
        }
        else {
            if (tile[rows][cols].has_player == 1 && cols < (BOX_COLS - 1) && tile[rows][cols + 1].has_box == 0 && tile[rows][cols + 2].is_wall == 0) {
                tile[rows][cols].has_player = 0;
                tile[rows][cols + 1].has_player = 1;
            }
        }
    }
}