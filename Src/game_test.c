#include "cprocessing.h"
#include "game_test_utils.h"

#define BOX_COLS 10
#define BOX_ROWS 10

#define WHITE CP_Color_Create(255, 255, 255, 255)
#define GRAY CP_Color_Create(150, 150, 150, 255)
#define BLACK CP_Color_Create(0, 0, 0, 255)
#define RED CP_Color_Create(255, 0, 0, 255)
#define BLUEGRAY CP_Color_Create(136, 176, 215, 255)

float tile_width, tile_height;

struct TILES {
    int has_player, has_box, is_objective, is_wall;
};

struct TILES tile[BOX_ROWS][BOX_COLS];

void Game_Test_Init()
{
    CP_System_SetFrameRate(60.0f);
    CP_System_SetWindowSize(900, 900);

    for (int row = 0; row < BOX_ROWS; ++row) {
        for (int col = 0; col < BOX_COLS; ++col) {
            tile[row][col].has_player = 0;
            tile[row][col].has_box = 0;
            tile[row][col].is_objective = 0;
            tile[row][col].is_wall = 0;
        }
    }

    CP_Settings_StrokeWeight(0.5);
    tile_width = (float)CP_System_GetWindowWidth() / (float)BOX_COLS;
    tile_height = (float)CP_System_GetWindowHeight() / (float)BOX_ROWS;

    tile[7][7].has_player = 1;
    tile[6][7].has_box = 1;
    tile[5][6].has_box = 1;
    tile[6][6].has_box = 1;
    tile[7][6].has_box = 1;
    tile[4][7].is_objective = 1;
    tile[7][4].is_objective = 1;

    tile[0][0].is_wall = 1;
    tile[0][1].is_wall = 1;
    tile[0][2].is_wall = 1;
    tile[0][3].is_wall = 1;
    tile[0][4].is_wall = 1;
    tile[0][5].is_wall = 1;
    tile[0][6].is_wall = 1;
    tile[0][7].is_wall = 1;
    tile[0][8].is_wall = 1;
    tile[0][9].is_wall = 1;
    tile[9][0].is_wall = 1;
    tile[9][1].is_wall = 1;
    tile[9][2].is_wall = 1;
    tile[9][3].is_wall = 1;
    tile[9][4].is_wall = 1;
    tile[9][5].is_wall = 1;
    tile[9][6].is_wall = 1;
    tile[9][7].is_wall = 1;
    tile[9][8].is_wall = 1;
    tile[9][9].is_wall = 1;
    tile[1][0].is_wall = 1;
    tile[2][0].is_wall = 1;
    tile[3][0].is_wall = 1;
    tile[4][0].is_wall = 1;
    tile[5][0].is_wall = 1;
    tile[6][0].is_wall = 1;
    tile[7][0].is_wall = 1;
    tile[8][0].is_wall = 1;
    tile[0][9].is_wall = 1;
    tile[1][9].is_wall = 1;
    tile[2][9].is_wall = 1;
    tile[3][9].is_wall = 1;
    tile[4][9].is_wall = 1;
    tile[5][9].is_wall = 1;
    tile[6][9].is_wall = 1;
    tile[7][9].is_wall = 1;
    tile[8][9].is_wall = 1;

}

void Game_Test_Update()
{
    float box_row = 0.0f, box_col = 0.0f;
    CP_Graphics_ClearBackground(GRAY);

    for (int rows = 0; rows < BOX_ROWS; rows++) {
        for (int cols = 0; cols < BOX_COLS; cols++) {
            draw_assets(tile[rows][cols], rows, cols, box_row, box_col, tile_width, tile_height);
            box_col += tile_width;
        }
        box_row += tile_height;
        box_col = 0.0f; //Reset for drawing of new row
    }

    for (int rows = 0; rows < BOX_ROWS; rows++) {
        for (int cols = 0; cols < BOX_COLS; cols++) {
            if (CP_Input_KeyTriggered(KEY_W)) {
                move_up(cols, rows, tile);
            }
            else if (CP_Input_KeyTriggered(KEY_A)) {
                move_left(cols, rows, tile);
            }
            else if (CP_Input_KeyTriggered(KEY_S)) {
                move_down(cols, rows, tile);
            }
            else if (CP_Input_KeyTriggered(KEY_D)) {
                move_right(cols, rows, tile);
            }
            box_col += tile_width;
        }
        box_row += tile_height;
        box_col = 0.0f; //Reset for drawing of new row
    }

    
}

void Game_Test_Exit()
{
    //Nothing to free
}