#pragma once

void overlay_pause();
int unpause(int game_pause);
void overlay_game_over(int gameover);
int game_over(int game_pause0);
void overlay_reset();
int reset_check(int reset_confirmed);
int overlay_welcome();
int welcome_done(int game_pause);
void overlay_end_game();
void end_game();
