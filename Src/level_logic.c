#pragma once
/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: level_logic.c
Purpose	: Function for level number logics
*/

/* Use global extern variable global_level */
extern int global_level;

/* Local File Variable 'temp_level' initialised to 0.
To be used when Selecting Level from Level Select Screen */
int temp_level = 0;

/* Assign the highest unlocked Level to 'global_level'. */
void set_level() {
	global_level = (temp_level > global_level) ? temp_level : global_level;
}

/* Assign value of 'global_level' (Highest Unlocked Level) to 'temp_level' &
assigns selected level from Select Level screen, 'lvl', to 'global_level' */
void set_temp_level(int lvl) {
	temp_level = global_level;
	global_level = lvl;
}

/* Increments 'global_level' to allow transition to the Next Level */
void next_level() {
	++global_level;
}

/* Resets 'global_level' back to 1 after game ends */
void reset_level() {
	global_level = 1;
}