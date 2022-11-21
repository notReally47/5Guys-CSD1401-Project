/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	:
File	:
Purpose	:
*/

#include "cprocessing.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "basegame.h"
#include "settings.h"
#include "structs.h"

int main(void)
{
	CP_Engine_SetNextGameState(splash_screen_init, splash_screen_update, splash_screen_exit);
	CP_Engine_Run();
	return 0;
}
