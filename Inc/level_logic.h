#pragma once

/* Can call this variable for level related logic(include this Header File to use this variable) */
extern int global_level;

/* Sets the latest unlocked level for the 'Play'/'Continue' button at Main Menu*/
void set_level();

/* Sets the level chosen by player for the 'Select Level' button at Main Menu*/
void set_temp_level(int lvl);

/* Set Next Level*/
void next_level();