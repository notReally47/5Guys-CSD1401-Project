/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	:
File	:
Purpose	:
*/

#include "cprocessing.h"
#include "stdio.h"
#include "stdlib.h"
#include "structs.h"
#include "defines.h"
#include "mechanics.h"
#include "spritesheet.h"

extern Config config;
CP_Image spritesheet, customer_spritesheet;
CP_Image spritesheet,background;
CP_Vector CustomerS[CUSTOMER_MAX],Player,Camera,Offset[3];
static float move,corneroffset;
static float frame = 64.0f;
int index,toggled;

/* loads all assets using spritesheet
* takes in cellsize for scaling of player/customer sprite scaling
* translation is calculated by the cellsize and the sprite dimensions to center itself in the cell
* 
* 
*/
void init_spritesheet(float* cellSize,int cameratoggle) {
	*cellSize = (float)(config.settings.resolutionHeight/((SOKOBAN_GRID_ROWS)/cameratoggle));
	// Sprite Dimensions - Scale sprite based on cellSize
	Offset[0] = CP_Vector_Scale(CP_Vector_Set(frame,frame),*cellSize/(frame));
	// Offset[1]lation - Align sprite placement to the center of the cell
	Offset[1] = CP_Vector_Scale(CP_Vector_Set((*cellSize-Offset[0].x),(*cellSize-Offset[0].y)),0.5f);
	// Camera Translation - Aligns camera to player's position
	Offset[2] = CP_Vector_Set((float)(SOKOBAN_GRID_COLS/(cameratoggle*2)),(float)(SOKOBAN_GRID_ROWS/(cameratoggle*2)));
	corneroffset=*cellSize*(17.f/64.f);
	
	move = (float)((int)*cellSize / (CP_System_GetFrameRate()/3.f));
}
void load_spritesheet(float* cellSize,int cameratoggle) {
	load_background();
	spritesheet = CP_Image_Load("./Assets/Spritesheet/spritesheet.png");
	customer_spritesheet = CP_Image_Load("./Assets/Spritesheet/customer_spritesheet.png");
	if (spritesheet == NULL) {
		fprintf(stderr,"Unable to open spritesheet.png\n");
		CP_Image_Free(&spritesheet);
		exit(1);
	}
	else {
		// i have no idea what these do
		//CP_Settings_AntiAlias(TRUE);
		CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);
		//CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_MIRROR);
		CP_Settings_ImageMode(CP_POSITION_CORNER); // Required to align tiles
		//CP_Settings_BlendMode(CP_BLEND_ALPHA);
		index = 0;
		toggled = 0;
		init_spritesheet(cellSize,cameratoggle);	// Separated initialisation to allow camera toggle
	}
		
}
void load_background(void) {
	background = CP_Image_Load("./Assets/backgroundEmpty.png");
	if (background == NULL) {
		fprintf(stderr, "Unable to open backgroundEmpty.png\n");
		CP_Image_Free(&spritesheet);
		exit(1);
	}
	CP_Settings_ImageMode(CP_POSITION_CORNER);
}

/* cellx and celly scales off player's position x and y. allows for a more accurate animation when moving the player
* when variable face is 0, the player will re-initialise its position. this is called at the start and whenever player undo their move
* 
*/
int draw_player(float cellSize,float cellAlign,int playerPosX,int playerPosY,int face,int cameratoggle) {
	float cellx = cellSize*(float)playerPosY+cellAlign;
	float celly = cellSize*(float)playerPosX+cellSize*0.85f;
	int isAnimating = 0;
	static float elapsed = 0.f;
	elapsed += CP_System_GetDt();
	if (elapsed >= 0.2f) {
		index = (index+1)%3;
		elapsed = 0.f;
	}

	if (cameratoggle==(toggled+1)){
		Player = CP_Vector_Set(cellx,celly);
		toggled = !toggled;
	}
	//if (infected[10]==1)
	//	CP_Settings_Tint(CP_Color_Create(0,255,0,250));
	//else 
	//	CP_Settings_NoTint();
	CP_Settings_Translate(Offset[1].x,Offset[1].y);
	switch (face) {
	case 0: // starting position
		Player = CP_Vector_Set(cellx,celly);
	case SOKOBAN_DOWN:
		if (teleporter[1] == 1) {
			Player = CP_Vector_Set(cellx,cellSize*(float)(playerPosX-1)+cellSize*0.85f);
			teleporter[1] = 0;
		}
		if (Player.y < celly) {
			isAnimating = 1;
			Player.y += move;
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,index*frame,0.f,(index+1)*frame,frame,255);
		} 
		else
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,frame,0.f,frame*2.f,frame,255);
		break;
	case SOKOBAN_UP:
		if (teleporter[1] == 1) {
			Player = CP_Vector_Set(cellx,cellSize*(float)(playerPosX+1)+cellSize*0.85f);
			teleporter[1] = 0;
		}
		if (Player.y > celly) {
			isAnimating = 1;
			Player.y -= move;
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,(index+3)*frame,0.f,(index+4)*frame,frame,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,frame*4.f,0.f,frame*5.f,frame,255); 
		break;
	case SOKOBAN_LEFT:
		if (teleporter[1] == 1) {
			Player = CP_Vector_Set(cellSize*(float)(playerPosY+1)+cellAlign,celly);
			teleporter[1] = 0;
		}
		if (Player.x > cellx) {
			isAnimating = 1;
			Player.x -= move;
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,(index+3)*frame,frame,(index+4)*frame,(frame-1.f)*2.f,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,frame*4.f,frame,frame*5.f,(frame-1.f)*2.f,255); 
		break;
	case SOKOBAN_RIGHT:
		if (teleporter[1] == 1) {
			Player = CP_Vector_Set(cellSize*(float)(playerPosY-1)+cellAlign,celly);
			teleporter[1] = 0;
		}
		if (Player.x < cellx) {
			isAnimating = 1;
			Player.x += move;
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,index*frame,frame,(index+1)*frame,(frame-1.f)*2.f,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,Player.x,Player.y,Offset[0].x,Offset[0].y,frame,frame,frame*2.f,(frame-1.f)*2.f,255);
		break;
	}
	//CP_Settings_NoTint();
	CP_Settings_Translate(-Offset[1].x,-Offset[1].y);

	return isAnimating;
}

void draw_boarder(float cellX, float cellY, float cellSize, int row, int col) {
	if (row==0 && col==0){ // top left corner
		CP_Image_DrawSubImage(spritesheet,cellX+cellSize-corneroffset,cellY,corneroffset,cellSize+corneroffset,0.f,256.f,18.f,320.f,240);
		CP_Image_DrawSubImage(spritesheet,cellX+cellSize-corneroffset,cellY-corneroffset,corneroffset,cellSize,36.f,256.f,54.f,320.f,240); 
	}
	else if (row==0 && col==SOKOBAN_GRID_COLS-1){ // top right corner
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,corneroffset,cellSize+corneroffset,64.f,256.f,82.f,320.f,240);
		CP_Image_DrawSubImage(spritesheet,cellX,cellY-corneroffset,corneroffset,cellSize,100.f,256.f,118.f,320.f,240);
	}
	else if (col==0 && row==SOKOBAN_GRID_ROWS-1) // bottom left corner
		CP_Image_DrawSubImage(spritesheet,cellX+cellSize-corneroffset,cellY-cellSize+corneroffset,corneroffset,cellSize,18.f,256.f,36.f,320.f,240);
	else if (col==SOKOBAN_GRID_COLS-1 && row==SOKOBAN_GRID_ROWS-1) // bottom right corner
		CP_Image_DrawSubImage(spritesheet,cellX,cellY-cellSize+corneroffset,corneroffset,cellSize,82.f,256.f,100.f,320.f,240);
	else if (col==0) // left row
		CP_Image_DrawSubImage(spritesheet,cellX+cellSize-corneroffset,cellY,corneroffset,cellSize+ (cellSize / (frame)),0.f,256.f,18.f,320.f,240); 
	else if (row==0 && col>=SOKOBAN_GRID_COLS/3 && col<=SOKOBAN_GRID_COLS*2/3){ // top row fridge
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize+ (cellSize / (frame)),cellSize,320.f,128.f,384.f,192.f,240); 
		CP_Image_DrawSubImage(spritesheet,cellX,cellY-corneroffset,cellSize + (cellSize / (frame)),corneroffset,320.f,192.f,384.f,210.f,240);
	}
	else if (row==0){ // top row wallpaper
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize+ (cellSize / (frame)),cellSize,256.f,192.f,320.f,256.f,240); 
		CP_Image_DrawSubImage(spritesheet,cellX,cellY-corneroffset,cellSize+ (cellSize / (frame)),corneroffset,320.f,192.f,384.f,210.f,240);
	}
	else if (row==SOKOBAN_GRID_ROWS-1) // bottom row
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize + (cellSize / (frame)),corneroffset,320.f,238.f,384.f,256.f,240);
	else if (col==SOKOBAN_GRID_COLS-1) // right row
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,corneroffset,cellSize + (cellSize / (frame)),64.f,256.f,82.f,320.f,240);
}
void draw_box(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,64.f,128.f,128.f,192.f,255);
}
void draw_key(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,192.f,128.f,256.f,192.f,255);
}
void draw_key_in_box(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,frame*2.f,128.f,frame*3.f,192.f,255);
}
void draw_floor(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize + (cellSize / (frame)), cellSize + (cellSize / (frame)), 0.f, 128.f, 64.f, 192.f, 250);
}
/* moves the whole canvas with the player when moving. allow for a zoom-in side scrolling effect
*/

void draw_customer(float cellSize,float cellAlign,int customerPosX,int customerPosY,int customerdirection,int customernumber,int cameratoggle) {
	float cellx = cellSize*(float)customerPosY+cellAlign;
	float celly = cellSize*(float)customerPosX+cellSize*0.85f;

	// cellSize is for when the customer moves
	int i = customernumber;
	if (cameratoggle==(toggled+1))
		CustomerS[i] = CP_Vector_Set(cellx,celly);
	
	//(infected[i]==1)?CP_Settings_Tint(CP_Color_Create(0,255,0,250)):CP_Settings_NoTint(); // infected[1] means customer[1] is infected
	CP_Settings_Translate(Offset[1].x,Offset[1].y);
	
	switch (customerdirection) {
	case SOKOBAN_DOWN: // down
		if (CustomerS[i].y < celly) {
			CustomerS[i].y += move;
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,index*frame,0.f,(index+1)*frame,frame,255);
		} 
		else
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,frame,0.f,frame*2.f,frame,255);
		break;
	case SOKOBAN_UP: // up
		if (CustomerS[i].y > celly) {
			CustomerS[i].y -= move;
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,(index+3)*frame,0.f,(index+4)*frame,frame,255);
		}
		else
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,frame*4.f,0.f,frame*5.f,frame,255);
		break;
	case SOKOBAN_LEFT: // left
		if (CustomerS[i].x > cellx) {
			CustomerS[i].x -= move;
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,(index+3)*frame,frame,(index+4)*frame,(frame-1.f)*2.f,255);
		}
		else
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,frame*4.f,frame,frame*5.f,(frame-1.f)*2.f,255); 
		break;
	case SOKOBAN_RIGHT: // right
		if (CustomerS[i].x < cellx) {
			CustomerS[i].x += move;
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,index*frame,frame,(index+1)*frame,(frame-1.f)*2.f,255);
		}
		else
			CP_Image_DrawSubImage(customer_spritesheet,CustomerS[i].x,CustomerS[i].y,Offset[0].x,Offset[0].y,frame,frame,frame*2.f,(frame-1.f)*2.f,255);
		break;
	}
	CP_Settings_Translate(-Offset[1].x,-Offset[1].y);
	//CP_Settings_NoTint();
}
void draw_mecha(float cellX,float cellY,float cellSize) {
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,257.f,128.f,319.f,192.f,240); 
}
void draw_shelf(float cellX, float cellY, float cellSize,int index) {
	if (index==1)
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,128.f,256.f,192.f,320.f,250);
	else 
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,192.f,256.f,256.f,320.f,250);
}
void draw_teleporter(float cellX,float cellY,float cellSize) {
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,index*frame,frame*3.f,(index+1)*frame,frame*4.f,255);
}
void world_camera(float cellSize,float cellAlign,int playerRow,int playerCol,int face,int cameratoggle) {
	float cellx = cellSize*(float)playerCol+cellAlign;
	float celly = cellSize*(float)playerRow+cellSize*0.85f;
	int xoffset = playerCol-Offset[2].x;
	int yoffset = playerRow-Offset[2].y;
	if (cameratoggle==(toggled+1))
		Camera = CP_Vector_Set((float)xoffset*cellSize,(float)yoffset*cellSize);
	switch (face) {
	case SOKOBAN_UP: // up
		if (teleporter[1] == 1) 
			Camera = CP_Vector_Set((float)xoffset*cellSize,(float)(yoffset+1)*cellSize);
		if (Player.y > celly) 
			Camera.y -= move;
		break;
	case SOKOBAN_LEFT: // left
		if (teleporter[1] == 1) 
			Camera = CP_Vector_Set((float)(xoffset+1)*cellSize,(float)(yoffset)*cellSize);
		if (Player.x > cellx)
			Camera.x -= move;
		break;
	case SOKOBAN_DOWN: // down
		if (teleporter[1] == 1) 
			Camera = CP_Vector_Set((float)xoffset*cellSize,(float)(yoffset-1)*cellSize);
		if (Player.y < celly)
			Camera.y += move;
		break;
	case SOKOBAN_RIGHT: //right
		if (teleporter[1] == 1) 
			Camera = CP_Vector_Set((float)(xoffset-1)*cellSize,(float)(yoffset)*cellSize);
		if (Player.x < cellx)
			Camera.x += move;
		break;
	}
	CP_Settings_Translate(-Camera.x,-Camera.y);
}
void revert_world_camera(void) {
	CP_Settings_Translate(Camera.x,Camera.y);
}
/* frees all assets from memory when called
*/
void draw_background(void) {
	CP_Graphics_ClearBackground(CLEAR);
	CP_Image_Draw(background,0.f,0.f,(float)config.settings.resolutionWidth,(float)config.settings.resolutionHeight,255);
}
void free_sprite(void) {
	CP_Image_Free(&spritesheet);
	CP_Image_Free(&customer_spritesheet);
	free_background();
}
void free_background(void) {
	CP_Image_Free(&background);
}
