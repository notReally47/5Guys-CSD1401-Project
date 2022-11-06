#pragma once

extern int infected[CUSTOMER+1],tele[UM];

void card_init(void);
void card_selection(int stage, CP_BOOL CP_Input_MouseClicked);
void customer_status(Customer customer[CUSTOMER]);
void card_deck(int* POS,int cards[], int* size);
void card_effect(int pos,int cards[]);
int player_status(int* isLocked);
void teleport_UM(void);
void wetsign_UM(void);