
/* Global Variable 'level' initialised to 0 whenever New Game */
extern int global_level;
//int global_level = 6;		// To Debug Level


/* Local File Variable 'temp_level' initialised to 0. 
To be used when Selecting Level from Level Select Screen */
int temp_level = 0;

/* Assign the highest unlocked Level to 'level'. */
void set_level() {
	global_level = (temp_level > global_level) ? temp_level : global_level;
}

/* Assign value of 'level' (Highest Unlocked Level) to 'temp_level' &
assigns selected level from Select Level screen, 'lvl', to 'level' */
void set_temp_level(int lvl) {
	temp_level = global_level;
	global_level = lvl;
}

/* Increments 'level' to allow transition to the Next Level */
void next_level() {
	++global_level;
}