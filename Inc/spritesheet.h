#pragma once

void load_spritesheet(float cellSize);
int draw_player(const float* cellSize,const float* cellAlign,const int* playerPosX,const int* playerPosY,const int* face);
void draw_boarder(float cellX,float cellY,float cellSize);
void draw_box(float cellX,float cellY,float cellSize);
void draw_key(float cellX,float cellY,float cellSize);
void draw_key_in_box(float cellX,float cellY,float cellSize);
void draw_floor(float cellX,float cellY,float cellSize);
void draw_customer(const float* cellSize,const float* cellAlign,const int* customerPosX,const int* customerPosY,const int* customerdirection,const int* customernumber);
void draw_mecha(void);
void draw_tele(void);
void world_camera(float cellSize,int face,int dir);
void free_sprite(void);
