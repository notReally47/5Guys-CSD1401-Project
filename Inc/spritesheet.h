#pragma once

void load_spritesheet(float cellSize);
void draw_player(float cellx,float celly,int face);
void draw_boarder(float cellX,float cellY,float cellSize);
void draw_box(float cellX,float cellY,float cellSize);
void draw_key(float cellX,float cellY,float cellSize);
void draw_key_in_box(float cellX,float cellY,float cellSize);
void draw_floor(float cellX,float cellY,float cellSize);
void world_camera(float cellSize,int face,int dir);
void free_sprite(void);
