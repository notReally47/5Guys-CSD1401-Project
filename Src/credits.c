/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
          Guo Yiming (yiming.guo@digipen.edu)
			- Design
			- Scrolling animation
File	: credits.c
Purpose	: Credits Page
*/

#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "spritesheet.h"
#include "easydraw.h"

float textSize, btnWidth, btnHeight, staticX, dynamicY[12];
CP_Sound click;
Button back;

extern Config config;

void Credits_Init() {
	/*INITIALISE VARIABLES*/
	float imgSize = (float)config.settings.resolutionHeight / 20.0f;
	textSize = (float)config.settings.resolutionHeight / 15.f;
	btnWidth = (float)config.settings.resolutionWidth * 0.2f;
	btnHeight = (float)config.settings.resolutionHeight * 1.f / 12.f;
	click = CP_Sound_Load("./Assets/Sound/SFX/Click.wav");
	setButton(&back, "./Assets/UI/Back.png", imgSize / 2.0f + PADDING, imgSize / 2.0f + PADDING, imgSize, imgSize, YES);
	staticX = (float)config.settings.resolutionWidth / 2;
	float yPos = .75f * textSize;
	float temp = textSize;
	for (int i = 0; i < sizeof(dynamicY) / sizeof(dynamicY[0]); i++, temp += textSize) {
		dynamicY[i] = yPos + temp;
	}
	load_background();
	CP_Settings_ImageMode(CP_POSITION_CORNER);
}
void Credits_Update() {
	/*INITIALISE VARIABLES*/
	float speed = 100.f * CP_System_GetDt();
	for (int i = 0; i < sizeof(dynamicY) / sizeof(dynamicY[0]); i++) {
		dynamicY[i] -= speed;
		if (dynamicY[i] <= 0) {
			dynamicY[i] = (float)config.settings.resolutionHeight;
		}
	}

	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	/*INPUTS*/
	/*Return to Main Menu*/
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mousePos.x, mousePos.y)) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	/*DRAWING*/
	/*Clear Background/Tint & Draws Background art*/
	draw_background();
	CP_Settings_NoTint();
	
	/*Credits*/
	CP_Settings_TextSize(.5f * textSize);
	drawAlignedText(FADERBLACK, CENTER, "Shafiq Mirza Bin Mohamed Zahid", staticX, dynamicY[1]);
	drawAlignedText(FADERBLACK, CENTER, "Muhammad Faliq Bin Al-Hakim", staticX, dynamicY[2]);
	drawAlignedText(FADERBLACK, CENTER, "Jerell Tan Jian Yu", staticX, dynamicY[3]);
	drawAlignedText(FADERBLACK, CENTER, "Ian Chua Rong Bin", staticX, dynamicY[4]);
	drawAlignedText(FADERBLACK, CENTER, "Guo Yiming", staticX, dynamicY[5]);
	drawAlignedText(FADERBLACK, CENTER, "Cheng Ding Xiang", staticX, dynamicY[7]);
	drawAlignedText(FADERBLACK, CENTER, "Gerald Wong", staticX, dynamicY[8]);
	drawAlignedText(FADERBLACK, CENTER, "Claude Comair", staticX, dynamicY[10]);
	drawAlignedText(FADEBLACK, CENTER, "All content (c) 2022 DigiPen Institute of Technology Singapore, all rights reserved.", staticX, dynamicY[11]);
	drawAlignedText(BLACK, CENTER, "All content (c) 2022 DigiPen Institute of Technology Singapore, all rights reserved.", staticX + .035f * textSize, dynamicY[11]);

	/*Credit Headers*/
	CP_Settings_TextSize(textSize);
	drawAlignedText(BLACK, CENTER, "TEAM MEMBERS", staticX, dynamicY[0]);
	drawAlignedText(BLACK, CENTER, "INSTRUCTORS", staticX, dynamicY[6]);
	drawAlignedText(BLACK, CENTER, "PRESIDENT", staticX, dynamicY[9]);

	/*Draw Back Button*/
	drawButton(back);
}

void Credits_Exit() {
	CP_Image_Free(&back.img);
	free_background();
}
