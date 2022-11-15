#pragma once

extern int infected[CUSTOMER_MAX+1], teleporter[SETTINGS];
extern UniqueCards UM;

void card_init(void);
void card_selection(int stage, int *applied);
void customer_status(Customer customer[CUSTOMER_MAX]);
void card_deck(int* selectedpos, int* notselectedpos,int cards[], int* size);
void card_effect(int pos,int cards[], int stage);
int player_status(int* isLocked);
void teleport_UM(void);
void wetsign_UM(void);
void mechanic_flags(void);
void free_buttons(void);
