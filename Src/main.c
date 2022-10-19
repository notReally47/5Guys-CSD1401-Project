#include "cprocessing.h"
#include "splashscreen.h"
#include "basegame.h"

int main(void)
{
	//CP_Engine_SetNextGameState(splash_screen_init,splash_screen_update,splash_screen_exit);
	// skip to basegame
	CP_Engine_SetNextGameState(base_Init,base_Update,base_Exit);
	CP_Engine_Run();
	return 0;
}
