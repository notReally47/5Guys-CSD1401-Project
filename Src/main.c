#include "cprocessing.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "basegame.h"

int main(void)
{
	//CP_Engine_SetNextGameState(splash_screen_init,splash_screen_update,splash_screen_exit);
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	return 0;
}
