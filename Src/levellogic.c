
/* Global Variable 'level' initialised to 0 whenever New Game */
int level = 1;

/* Local File Variable 'temp_level' initialised to 0. 
To be used when Selecting Level from Level Select Screen*/
int temp_level = 0;

/* Assign the highest unlocked Level to 'level'. */
void set_level() {
	level = (temp_level > level) ? temp_level : level;
}

/* Assign value of 'level' (Highest Unlocked Level) to 'temp_level' &
assigns selected level from Select Level screen, 'lvl' to 'level'*/
void set_temp_level(int lvl) {
	temp_level = level;
	level = lvl;
}

/* Increments 'level' to allow transition to the Next Level*/
int next_level() {
	printf("Next Level: %d\n", level+1);
	return ++level;
}