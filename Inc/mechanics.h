#pragma once

extern int infected[CUSTOMER_MAX+1],teleporter[SETTINGS];

void card_init(void);
void card_selection(int stage, int* selected);
void customer_status(Customer customer[CUSTOMER_MAX]);
void card_deck(int* POS,int cards[], int* size);
void card_effect(int pos,int cards[]);
int player_status(int* isLocked);
void teleport_UM(void);
void wetsign_UM(void);