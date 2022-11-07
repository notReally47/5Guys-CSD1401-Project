#include "cprocessing.h"
#include "stdio.h"
#include "structs.h"
#include "defines.h"
#include "mechanics.h"

CP_Image spritesheet;
CP_Vector anim,dimen,trans,camera;
static float global_move;
const float frame = 64.0f;
int index,x,y;

/* loads all assets using spritesheet
* takes in cellsize for scaling of player/customer sprite scaling
* translation is calculated by the cellsize and the sprite dimensions to center itself in the cell
* 
* 
*/
void load_spritesheet(float cellSize) {
	spritesheet = CP_Image_Load("./Assets/Spritesheet/spritesheet.png");
	if (spritesheet == NULL) {
		fprintf(stderr,"Unable to open spritesheet.png\n");
		CP_Image_Free(&spritesheet);
		exit(1);
	}
	dimen = CP_Vector_Scale(CP_Vector_Set(frame,frame),cellSize/frame);
	trans = CP_Vector_Scale(CP_Vector_Set((cellSize-dimen.x),(cellSize-dimen.y)),0.5f);
	camera = CP_Vector_Set(0.f, 0.f);
	global_move = (float)((int)cellSize/10);
	index = 0;
	x = 1;
	y = 1;
	// i have no idea what these do
	CP_Settings_AntiAlias(TRUE);
	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_LINEAR);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	
	
}
/* cellx and celly scales off player's position x and y. allows for a more accurate animation when moving the player
* when variable face is 0, the player will re-initialise its position. this is called at the start and whenever player undo their move
* 
*/
void draw_player(float cellSize,float cellAlign,int playerPosX,int playerPosY,int face) {
	float cellx = cellSize*(float)playerPosY+cellAlign;
	float celly = cellSize*(float)playerPosX;
	static float elapsed = 0.f;
	elapsed += CP_System_GetDt();
	if (elapsed >= 0.2f) {
		index = (index+1)%3;
		elapsed = 0.f;
	}

	CP_Settings_Translate(trans.x,trans.y);
	switch (face) {
	case 0: // starting position
		anim = CP_Vector_Set(cellx,celly);
	case 3: // face down
		if (tele[5] == 1) {
			anim = CP_Vector_Set(cellx,cellSize*(float)(playerPosX-1));
			tele[5] = 0;
		}
		if (anim.y < celly) {
			anim.y += global_move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,index*frame,0.f,(index+1)*frame,frame,255);
		} 
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame,0.f,frame*2.f,frame,255);
		break;
	case 1: // face up
		if (tele[5] == 1) {
			anim = CP_Vector_Set(cellx,cellSize*(float)(playerPosX+1));
			tele[5] = 0;
		}
		if (anim.y > celly) {
			anim.y -= global_move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,(index+3)*frame,0.f,(index+4)*frame,frame,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame*4.f,0.f,frame*5.f,frame,255); 
		break;
	case 2: // face left
		if (tele[5] == 1) {
			anim = CP_Vector_Set(cellSize*(float)(playerPosY+1)+cellAlign,celly);
			tele[5] = 0;
		}
		if (anim.x > cellx) {
			anim.x -= global_move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,(index+3)*frame,frame,(index+4)*frame,frame*2.f,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame*4.f,frame,frame*5.f,frame*2.f,255); 
		break;
	case 4: // face right
		if (tele[5] == 1) {
			anim = CP_Vector_Set(cellSize*(float)(playerPosY-1)+cellAlign,celly);
			tele[5] = 0;
		}
		if (anim.x < cellx) {
			anim.x += global_move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,index*frame,frame,(index+1)*frame,frame*2.f,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame,frame,frame*2.f,frame*2.f,255);
		break;
	}
	CP_Settings_Translate(-trans.x,-trans.y);
}

void draw_boarder(float cellX, float cellY, float cellSize) {
	//if (cellY == 0.f || cellY*SOKOBAN_GRID_ROWS)
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,320.f,128.f,384.f,192.f,240);
	//else if (cellX == 0.f || cellX*SOKOBAN_GRID_COLS)
		//CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,320.f,128.f,384.f,192.f,255);
}
void draw_box(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,64.f,128.f,128.f,192.f,255);
}
void draw_key(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,256.f,128.f,320.f,192.f,255);
}
void draw_key_in_box(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,frame*2.f,128.f,frame*3.f,192.f,255);
}
void draw_floor(float cellX,float cellY,float cellSize){
	CP_Image_DrawSubImage(spritesheet,cellX,cellY,cellSize,cellSize,0.f,128.f,64.f,192.f,250);
}
/* moves the whole canvas with the player when moving. allow for a zoom-in side scrolling effect
*/
void draw_customer(float cellX,float cellY,float cellSize,int customerdirection) {
	// cellSize is for when the customer moves
	switch (customerdirection) {
	case 3:
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,dimen.x,dimen.y,frame,0.f,frame*2.f,frame,255);
		break;
	case 1:
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,dimen.x,dimen.y,frame*4.f,0.f,frame*5.f,frame,255); 
		break;
	case 2:
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,dimen.x,dimen.y,frame*4.f,frame,frame*5.f,frame*2.f,255);
		break;
	case 4:
		CP_Image_DrawSubImage(spritesheet,cellX,cellY,dimen.x,dimen.y,frame,frame,frame*2.f,frame*2.f,255);
		break;
	}
}
void draw_mecha(void) {
	// to do
}
void draw_tele(void) {
	// to do
}
void world_camera(float cellSize, int face, int dir) {
	switch (face) {
	case 1: // up
		y = (dir==1) ? y+1:y;
		if (camera.y < cellSize*y)
			camera.y += global_move;
		break;
	case 2: // left
		x = (dir==2) ? x+1:x;
		if (camera.x < cellSize*x)
			camera.x += global_move;
		break;
	case 3: // down
		y = (dir==3) ? y-1:y;
		if (camera.y > cellSize*y)
			camera.y -= global_move;
		break;
	case 4: //right
		x = (dir==4) ? x-1:x;
		if (camera.x > cellSize*x)
			camera.x -= global_move;
		break;
	}
	CP_Settings_Translate(camera.x,camera.y);
}
/* frees all assets from memory when called
*/
void free_sprite(void) {
	CP_Image_Free(&spritesheet);
}
