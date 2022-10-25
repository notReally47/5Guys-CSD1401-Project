#include "CProcessing.h"
#include "mainmenu.h"
#include "utils.h"

#define btnWidth	windowsWidth * 0.1f
#define btnHeight	windowsHeight * 1.f / 12.f

float windowsWidth;
float windowsHeight;

void Credits_Init() {
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
	windowsWidth = (float)CP_System_GetWindowWidth();
	windowsHeight = (float)CP_System_GetWindowHeight();
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(18);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// Credit List
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText("Team Members:", windowsWidth / 2, 100);
	CP_Font_DrawText("Muhammad Faliq Bin Al-Hakim", windowsWidth / 2, 120);
	CP_Font_DrawText("Shafiq Mirza Bin Mohamed Zahid", windowsWidth / 2, 140);
	CP_Font_DrawText("Ian Chua Rong Bin", windowsWidth / 2, 160);
	CP_Font_DrawText("Jerell Tan Jian Yu", windowsWidth / 2, 180);
	CP_Font_DrawText("Guo Yiming", windowsWidth / 2, 200);
	CP_Font_DrawText("-------------------------------", windowsWidth / 2, 220);
	CP_Font_DrawText("Instructors:", windowsWidth / 2, 240);
	CP_Font_DrawText("Ding Xiang Cheng", windowsWidth / 2, 260);
	CP_Font_DrawText("Gerald Wong", windowsWidth / 2, 280);
	CP_Font_DrawText("-------------------------------", windowsWidth / 2, 300);
	CP_Font_DrawText("President:", windowsWidth / 2, 320);
	CP_Font_DrawText("Claude Comair", windowsWidth / 2, 340);
	CP_Font_DrawText("-------------------------------", windowsWidth / 2, 360);
	CP_Font_DrawText("All content (c) 2021 DigiPen Institute of Technology Singapore, all rights reserved.", windowsWidth / 2, 380);
}

void Credits_Update() {
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(windowsWidth / 2, 500, btnWidth, btnHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	CP_Settings_Fill(CP_Color_Create(200, 50, 50, 255));
	CP_Graphics_DrawRect(windowsWidth / 2, 500, btnWidth, btnHeight);

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Back", windowsWidth / 2, 500);
}

void Credits_Exit() {

}
