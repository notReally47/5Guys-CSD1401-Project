#include "cprocessing.h"
#include "mainmenu.h"
CP_Image img; // declare variable img

void splash_screen_init(void)
{
    img = CP_Image_Load("./Assets/DigiPen_BLACK.png"); // load digipen screen logo png graphics into variable img
    CP_System_SetWindowSize(CP_System_GetDisplayWidth()>>1,CP_System_GetDisplayHeight()>>1); // set window size to half of display height/width
}

void splash_screen_update(void)
{   
    // draw the logo -- set alpha to frame count multiplied by 2 to reach 240 frames in ~4 seconds
    CP_Image_Draw(img,(float)(CP_System_GetWindowWidth()>>1),(float)(CP_System_GetWindowHeight()>>1),(float)CP_Image_GetWidth(img),(float)CP_Image_GetHeight(img),(CP_System_GetFrameCount()<<1)%240);
    CP_Graphics_ClearBackground(CP_Color_Create(0,0,0,255));
    // transition to next state after 4 secs
    ((CP_System_GetFrameCount()<<1)%240 == 0) ? CP_Engine_SetNextGameState(Main_Menu_Init,Main_Menu_Update,Main_Menu_Exit):0;
    // todo? fade out before transition
}

void splash_screen_exit(void)
{   
    CP_Image_Free(&img); // unload the img
}
