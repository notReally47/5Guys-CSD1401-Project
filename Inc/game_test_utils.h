#pragma once

#define BOX_COLS 10
#define BOX_ROWS 10

void draw_assets(struct TILES tile, int rows, int cols, float box_row, float box_col, float tile_width, float tile_height);
void move_up(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]);
void move_down(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]);
void move_left(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]);
void move_right(int cols, int rows, struct TILES tile[BOX_ROWS][BOX_COLS]);