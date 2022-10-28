#include "cprocessing.h"
#include "stdio.h"
#include "structs.h"
#include "defines.h"

CP_Image spritesheet;
CP_Vector anim,dimen,trans;

static float elapsed,move;
static const float frame = 64.0f;
static int index,box;

void load_spritesheet(float cellSize) {
	spritesheet = CP_Image_Load("./Assets/Spritesheet/spritesheet.png");
	if (spritesheet == NULL) {
		printf("Unable to open spritesheet.png\n");
		CP_Image_Free(&spritesheet);
		exit(1);
	}
	dimen = CP_Vector_Scale(CP_Vector_Set(frame,frame),cellSize/frame);
	trans = CP_Vector_Scale(CP_Vector_Set((cellSize-dimen.x),(cellSize-dimen.y)),0.5f);
	move = (float)((int)cellSize/10);
	index = 0;
	box = 0;
	elapsed = 0.0f;
	// i have no idea what these do
	CP_Settings_AntiAlias(TRUE);
	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_LINEAR);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	
	
}


void draw_player(float cellX,float cellY,int face) {
	elapsed += CP_System_GetDt();
	if (elapsed >= 0.2f) {
		index = (index+1)%3;
		elapsed = 0.0f;
	}
	
	CP_Settings_Translate(trans.x,trans.y);
	switch (face) {
	case 0: // starting position
		anim = CP_Vector_Set(cellX,cellY);
	case 3: // face down
		if (anim.y < cellY) {
			anim.y += move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,index*frame,0.f,(index+1)*frame,frame,255);
		} 
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame,0.f,frame*2.f,frame,255);
		break;
	case 1: // face up
		if (anim.y > cellY) {
			anim.y -= move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,(index+3)*frame,0.f,(index+4)*frame,frame,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame*4.f,0.f,frame*5.f,frame,255); 
		break;
	case 2: // face left
		if (anim.x > cellX) {
			anim.x -= move;
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,(index+3)*frame,frame,(index+4)*frame,frame*2.f,255);
		}
		else
			CP_Image_DrawSubImage(spritesheet,anim.x,anim.y,dimen.x,dimen.y,frame*4.f,frame,frame*5.f,frame*2.f,255); 
		break;
	case 4: // face right
		if (anim.x < cellX) {
			anim.x += move;
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

void free_sprite(void) {
	CP_Image_Free(&spritesheet);
}
