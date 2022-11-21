#pragma once
/*
* All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Shafiq Mirza Bin Mohamed Zahid (shafiqmirza.b@digipen.edu)
File	: mechanics.h
Purpose	: Declaration of functions load_spritesheet, load_background, init_spritesheet, draw_player, draw_boarder, draw_box, draw_key, draw_key_in_box,
		  draw_floor, draw_customer, draw_mecha, draw_shelf, draw_teleporter, draw_background, world_camera, revert_world_camera, free_sprite, free_background
*/

// loads the spritesheet which contains all the player/customer/environmental sprites
void load_spritesheet(float* cellSize,int cameratoggle);
// loads the background art
void load_background(void);
// initialise variables, and particularly cellSize which updates if camera is zoomed in/out
void init_spritesheet(float* cellSize,int cameratoggle);
// renders the player sprite
int draw_player(float cellSize,float cellAlign,int playerPosX,int playerPosY,int face,int cameratoggle,int game_pause);
// renders the boarder/fridge/wallpaper sprites
void draw_boarder(float cellX,float cellY,float cellSize,int row,int col);
// renders the box sprite
void draw_box(float cellX,float cellY,float cellSize);
// renders the key objective sprite
void draw_key(float cellX,float cellY,float cellSize);
// renders the box inside the key objective sprite
void draw_key_in_box(float cellX,float cellY,float cellSize);
// renders the tile floor sprite
void draw_floor(float cellX,float cellY,float cellSize);
// renders the customer sprite
void draw_customer(float cellSize,float cellAlign,int customerPosX,int customerPosY,int customerdirection,int customernumber,int cameratoggle,int game_pause);
// renders the wetsign sprite if flag is true
void draw_mecha(float cellX,float cellY,float cellSize);
// renders various shelf sprites
void draw_shelf(float cellX, float cellY, float cellSize,int shelf);
// renders the teleporter sprites that animates if flag is true
void draw_teleporter(float cellX, float cellY, float cellSize);
// renders the background art
void draw_background(void);
// function that enables the player to zoom in on the map which allows the player to focus on the area surrounding the player sprite
void world_camera(float cellSize,float cellAlign,int playerRow,int playerCol,int face,int cameratoggle);
// reverts back the translated canvas for overlays
void revert_world_camera(void);
// frees spritesheet
void free_sprite(void);
// frees background art
void free_background(void);
