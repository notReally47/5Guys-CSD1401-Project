/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
			- Clicking SFX
          Guo Yiming (yiming.guo@digipen.edu)
			- Drawing
File	: credits.c
Purpose	: Credits Page
*/

#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "spritesheet.h"
#include "easydraw.h"

float textSize, btnWidth, btnHeight, staticXleft, staticXright, dynamicYleft[10], dynamicYright[9];
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
	staticXleft = (float)config.settings.resolutionWidth / 4;
	staticXright = (float)config.settings.resolutionWidth - config.settings.resolutionWidth / 4;
	float yPos = CP_System_GetWindowHeight() / 4;
	float temp = 0.f;
	for (int i = 0; i < sizeof(dynamicYleft) / sizeof(dynamicYleft[0]); i++, temp += textSize - textSize / 3) {
		dynamicYleft[i] = yPos + temp;
	}
	temp = 0.f;
	for (int i = 0; i < sizeof(dynamicYright) / sizeof(dynamicYright[0]); i++, temp += textSize - textSize / 3) {
		dynamicYright[i] = yPos + temp;
	}
	load_background();
	CP_Settings_ImageMode(CP_POSITION_CORNER);
}
void Credits_Update() {
	/*INITIALISE VARIABLES*/
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
	drawAlignedText(FADERBLACK, CENTER, "Shafiq Mirza Bin Mohamed Zahid", staticXleft, dynamicYleft[1]);
	drawAlignedText(FADERBLACK, CENTER, "Muhammad Faliq Bin Al-Hakim", staticXleft, dynamicYleft[2]);
	drawAlignedText(FADERBLACK, CENTER, "Jerell Tan Jian Yu", staticXleft, dynamicYleft[3]);
	drawAlignedText(FADERBLACK, CENTER, "Ian Chua Rong Bin", staticXleft, dynamicYleft[4]);
	drawAlignedText(FADERBLACK, CENTER, "Guo Yiming", staticXleft, dynamicYleft[5]);
	drawAlignedText(FADERBLACK, CENTER, "Cheng Ding Xiang", staticXleft, dynamicYleft[8]);
	drawAlignedText(FADERBLACK, CENTER, "Gerald Wong", staticXleft, dynamicYleft[9]);
	drawAlignedText(FADERBLACK, CENTER, "Claude Comair", staticXright, dynamicYright[1]);
	drawAlignedText(FADERBLACK, CENTER, "Jason Chu   Christopher Comair   Michael Gats", staticXright, dynamicYright[4]);
	drawAlignedText(FADERBLACK, CENTER, "Michele Comair   Raymond Yan	Samir   Abou Samra", staticXright, dynamicYright[5]);
	drawAlignedText(FADERBLACK, CENTER, "Prasanna Ghali   John Bauer	Dr. Erik Mohrmann", staticXright, dynamicYright[6]);
	drawAlignedText(FADERBLACK, CENTER, "Melvin Gonsalvez   Angela Kugler   Dr. Charles Duba", staticXright, dynamicYright[7]);
	drawAlignedText(FADERBLACK, CENTER, "Ben Ellinger   Johnny Deek", staticXright, dynamicYright[8]);
	drawAlignedText(BLACK, CENTER, "Created at", CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() - 2.f * textSize);
	drawAlignedText(FADERBLACK, CENTER, "DigiPen Institute of Technology Singapore", CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() - 1.5f * textSize);
	drawAlignedText(FADEBLACK, CENTER, "All content (c) 2022 DigiPen Institute of Technology Singapore, all rights reserved.", CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() - textSize / 2);
	drawAlignedText(BLACK, CENTER, "All content (c) 2022 DigiPen Institute of Technology Singapore, all rights reserved.", CP_System_GetWindowWidth() / 2 + .035f * textSize, CP_System_GetWindowHeight() - textSize / 2);

	/*Credit Headers*/
	CP_Settings_TextSize(textSize);
	drawAlignedText(BLACK, CENTER, "TEAM MEMBERS", staticXleft, dynamicYleft[0]);
	drawAlignedText(BLACK, CENTER, "INSTRUCTORS", staticXleft, dynamicYleft[7]);
	drawAlignedText(BLACK, CENTER, "PRESIDENT", staticXright, dynamicYright[0]);
	drawAlignedText(BLACK, CENTER, "EXECUTIVES", staticXright, dynamicYright[3]);

	/*Draw Back Button*/
	drawButton(back);
}

void Credits_Exit() {
	CP_Image_Free(&back.img);
	free_background();
}
