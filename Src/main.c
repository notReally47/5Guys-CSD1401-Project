#include "cprocessing.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "basegame.h"

int main(void)
{
	//CP_Engine_SetNextGameState(splash_screen_init,splash_screen_update,splash_screen_exit);
<<<<<<< Updated upstream
	// skip to main menu
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	// skip to basegame
	//CP_Engine_SetNextGameState(base_Init,base_Update,base_Exit);
=======
	// skip to basegame for testing
	CP_Engine_SetNextGameState(base_Init,base_Update,base_Exit);
>>>>>>> Stashed changes
	CP_Engine_Run();
	return 0;
}
