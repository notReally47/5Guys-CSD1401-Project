#include "cprocessing.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "basegame.h"
#include "settings.h"
#include "structs.h"

/*
* Declare config variable to be used throughout the program.
* Use 'extern Config config;' to reference global config.
*/
Config config;

int main(void)
{
	/*Initalize config by reading from file, or creating the file if it does not exist.*/
	config = readFile();
	CP_Engine_SetNextGameState(splash_screen_init, splash_screen_update, splash_screen_exit);
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	return 0;
}
