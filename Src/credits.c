#include "CProcessing.h"
#include "mainmenu.h"
#include "utils.h"
#include "defines.h"
#include "options_draw.h"

float windowsWidth, windowsHeight, textSize, btnWidth, btnHeight;
CP_Sound click;
Button back;

void Credits_Init() {
	/*INITIALISE VARIABLES*/
	float imgSize = (float)(CP_System_GetWindowHeight() / 20.0f);
	windowsWidth = (float)CP_System_GetWindowWidth();
	windowsHeight = (float)CP_System_GetWindowHeight();
	textSize = windowsHeight / 20.0f;
	btnWidth = windowsWidth * 0.2f;
	btnHeight = windowsHeight * 1.f / 12.f;
	click = CP_Sound_Load("./Assets/Sound/SFX/Click.wav");
	setButton(&back, "./Assets/UI/Back.png", imgSize / 2.0f + PADDING, imgSize / 2.0f + PADDING, imgSize, imgSize, YES);
}

void Credits_Update() {
	/*INITIALISE VARIABLES*/
	CP_Vector highestPos = CP_Vector_Set(windowsWidth / 2, 3 * textSize);
	CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	/*INPUTS*/
	/*Return to Main Menu*/
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(back.position.x, back.position.y, back.btnWidth, back.btnHeight, mousePos.x, mousePos.y)) {
			CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	/*DRAWING*/

	/*Clear Background & Tint*/
	CP_Graphics_ClearBackground(GRAY);
	CP_Settings_NoTint();
	
	/*Credits*/
	CP_Settings_Fill(WHITE);
	CP_Settings_Stroke(WHITE);
	CP_Settings_TextSize(.75f * textSize);
	drawAlignedText(BLACK, CENTER, "Muhammad Faliq Bin Al-Hakim", highestPos.x, highestPos.y + textSize);
	drawAlignedText(BLACK, CENTER, "Shafiq Mirza Bin Mohamed Zahid", highestPos.x, highestPos.y + 2 * textSize);
	drawAlignedText(BLACK, CENTER, "Ian Chua Rong Bin", highestPos.x, highestPos.y + 3 * textSize);
	drawAlignedText(BLACK, CENTER, "Jerell Tan Jian Yu", highestPos.x, highestPos.y + 4 * textSize);
	drawAlignedText(BLACK, CENTER, "Guo Yiming", highestPos.x, highestPos.y + 5 * textSize);
	drawAlignedText(BLACK, CENTER, "Ding Xiang Cheng", highestPos.x, highestPos.y + 7 * textSize);
	drawAlignedText(BLACK, CENTER, "Gerald Wong", highestPos.x, highestPos.y + 8 * textSize);
	drawAlignedText(BLACK, CENTER, "Claude Comair", highestPos.x, highestPos.y + 10 * textSize);
	drawAlignedText(WHITE, CENTER, "BGM: Pixelland by Kevin MacLeod | https://incompetech.com/", highestPos.x, highestPos.y + 11 * textSize);
	drawAlignedText(WHITE, CENTER, "Music promoted by https://www.chosic.com/free-music/all/", highestPos.x, highestPos.y + 12 * textSize);
	drawAlignedText(WHITE, CENTER, "Creative Commons CC BY 3.0", highestPos.x, highestPos.y + 13 * textSize);
	drawAlignedText(WHITE, CENTER, "https://creativecommons.org/licenses/by/3.0/", highestPos.x, highestPos.y + 14 * textSize);
	drawAlignedText(BLACK, CENTER, "All content (c) 2021 DigiPen Institute of Technology Singapore, all rights reserved.", highestPos.x, highestPos.y + 15 * textSize);

	/*Credit Headers*/
	CP_Settings_TextSize(textSize);
	drawAlignedText(WHITE, CENTER, "TEAM MEMBERS:", highestPos.x, highestPos.y);
	drawAlignedText(WHITE, CENTER, "INSTRUCTORS:", highestPos.x, highestPos.y + 6 * textSize);
	drawAlignedText(WHITE, CENTER, "PRESIDENT:", highestPos.x, highestPos.y + 9 * textSize);

	/*Draw Back Button*/
	drawButton(back);
}

void Credits_Exit() {
	CP_Image_Free(&back.img);
}
