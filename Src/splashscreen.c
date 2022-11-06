#include "cprocessing.h"
#include "mainmenu.h"
#include "structs.h"
#include "defines.h"
// declare global variables: img,fade,x,y,width,height
CP_Image img;
int fade;
float x,y,width,height;
extern Config config;
CP_Sound gameMusic;

void splash_screen_init(void)
{
    gameMusic = CP_Sound_Load("./Assets/Sound/music.mp3");
    img = CP_Image_Load("./Assets/DigiPen_BLACK.png"); // load digipen screen logo png graphics into variable img

    if (config.settings.windowed) {
        CP_System_SetWindowSize(config.settings.resolutionWidth, config.settings.resolutionHeight); // set window size to half of display height/width
    }

    else {
        CP_System_Fullscreen();
    }
    
    fade = 0;
    x = CP_System_GetWindowWidth()>>1;
    y = CP_System_GetWindowHeight()>>1;
    width = (float)CP_System_GetWindowWidth()*0.8f;
    height = (float)CP_System_GetWindowHeight()*0.4f;
}

void splash_screen_update(void)
{   
    int timer = CP_System_GetFrameCount()<<2; // <<1 to reach 240 frame in ~4 seconds, <<2 to reach 240 frame in ~2 seconds
    (timer <= 240) ? // increase alpha while timer is less than 240
    CP_Image_Draw(img,x,y,width,height,++fade<<2):
    (timer > 240 && timer < 480) ? // decrease alpha while timer is more than 240, but less than 480
    CP_Image_Draw(img,x,y,width,height,--fade<<2):
    (timer >= 480) ? // transition to main menu after fading out
    CP_Engine_SetNextGameState(Main_Menu_Init,Main_Menu_Update,Main_Menu_Exit):0;
    CP_Graphics_ClearBackground(CP_Color_Create(0,0,0,255));
}

void splash_screen_exit(void)
{   
    CP_Sound_PlayAdvanced(gameMusic, config.settings.audio / 100.f, 1, YES, CP_SOUND_GROUP_MUSIC);
    CP_Image_Free(&img); // unload the img
}
