#pragma once

struct Pause_Size initialise_pause_size();
struct Game_Over_Size initialise_game_over_size();
void overlay_pause();
int unpause(int game_pause);
void overlay_game_over();
int game_over(int game_pause0);
