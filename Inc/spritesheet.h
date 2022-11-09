#pragma once

void load_spritesheet(float* cellSize,int cameratoggle);
void init_spritesheet(float* cellSize,int cameratoggle);
int draw_player(float cellSize,int playerPosX,int playerPosY,int face,int cameratoggle);
void draw_boarder(float cellX,float cellY,float cellSize);
void draw_box(float cellX,float cellY,float cellSize);
void draw_key(float cellX,float cellY,float cellSize);
void draw_key_in_box(float cellX,float cellY,float cellSize);
void draw_floor(float cellX,float cellY,float cellSize);
void draw_customer(float cellSize,int customerPosX,int customerPosY,int customerdirection,int customernumber,int cameratoggle);
void draw_mecha(void);
void draw_tele(void);
void world_camera(float cellSize,int playerRow,int playerCol,int face,int cameratoggle);
void free_sprite(void);
