#pragma once
/*
* All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Author	: Shafiq Mirza Bin Mohamed Zahid (shafiqmirza.b@digipen.edu)
File	: mechanics.h
Purpose	: Declaration of functions card_settings, card_init, card_selection, customer_status, card_deck, card_effect, player_status, teleport_UM, wetsign_UM, mechanics_flags, free_buttons
*/

// allows variables to be called in other source files
extern int infected[CUSTOMER_MAX+1], teleporter[SETTINGS];
// allows variables to be called in other source files
extern UniqueCards UM;
// initialise settings for rendering of cards
void card_settings(void);
// initialise settings for UM.flags and other variables
void card_init(void);
// allows player to select between a choice of two cards that's on display
void card_selection(int stage, int *applied);
// unique mechanic related function that 'infects' customer if mechanic flags is true
void customer_status(Customer customer[CUSTOMER_MAX]);
// a shuffling deck system that removes the selected card from the deck, and shuffles the next two cards
void card_deck(int* selectedpos, int* notselectedpos,int cards[], int* size);
// function that enables flags if card was selected in card_selection()
void card_effect(int pos,int cards[], int stage);
// unique mechanic related function that checks if player is 'infected' and periodically stuns the player to simulate coughing
void player_status(int* isLocked);
// enables the teleporter array
void teleport_UM(void);
// unique mechanic related function that scans the map for empty spots and places a wet sign. afterwards, randomly removes the wet signs until a minimum of 15 remains
void wetsign_UM(void);
// function that's called every level to check if certain flags are active, and if so, either calls the related function or manipulate variables
void mechanic_flags(void);
// frees sprite used in card_selection()
void free_buttons(void);
