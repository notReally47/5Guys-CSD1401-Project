#pragma once
/*
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: level_logic.h
Purpose	: Declaration of functions set_level(), set_temp_level(), next_level() & reset_level()
*/

/* Can call this variable for level related logic(include this Header File to use this variable) */
extern int global_level;

/* Sets the latest unlocked level for the 'Play'/'Continue' button at Main Menu*/
void set_level();

/* Sets the level chosen by player for the 'Select Level' button at Main Menu*/
void set_temp_level(int lvl);

/* Set Next Level*/
void next_level();

/* Reset Level When Game Ends */
void reset_level();
