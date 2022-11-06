#pragma once

void load_spritesheet(float cellSize);
void draw_player(float cellSize,float cellAlign,int playerPosX,int playerPosY,int face);
void draw_boarder(float cellX,float cellY,float cellSize);
void draw_box(float cellX,float cellY,float cellSize);
void draw_key(float cellX,float cellY,float cellSize);
void draw_key_in_box(float cellX,float cellY,float cellSize);
void draw_floor(float cellX,float cellY,float cellSize);
void draw_customer(float cellX,float cellY,float cellSize,int cusdirection);
void draw_mecha(void);
void draw_tele(void);
void world_camera(float cellSize,int face,int dir);
void free_sprite(void);
