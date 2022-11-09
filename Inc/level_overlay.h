#pragma once

struct Pause_Size initialise_pause_size();
void overlay_pause();
int unpause(int game_pause);
struct Game_Over_Size initialise_game_over_size();
void overlay_game_over();
int game_over(int game_pause0);
struct Reset_Size initialise_reset_size();
void overlay_reset();
int reset_check(int reset_confirmed);
