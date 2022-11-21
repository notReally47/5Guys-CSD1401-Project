#pragma once
/*
Author	: Muhammad Faliq Bin Al-Hakim (muhammadfaliq.b@digipen.edu)
File	: level_overlay.h
Purpose	: Declarations of Overlay Functions
*/

/* Render Pause Overlay */
void overlay_pause();

/* Unpause game when button/esc is clicked/pressed */
int unpause(int game_pause);

/* Render Game Over Overlay */
void overlay_game_over(int gameover);

/* Return to Main Menu when button is Clicked */
int game_over(int game_pause0);

/* Render Reset Overlay */
void overlay_reset();

/* Reset Map if YES is Clicked, return to game if No is Clicked */
int reset_check(int reset_confirmed);

/* Render Welcome Message Overlay */
int overlay_welcome();

/* Start Level 1 when Button is Clicked */
int welcome_done(int game_pause);

/* Renders Game End Overlay */
void overlay_end_game();

/* Return to Main Menu when Button is Clicked */
void end_game();
