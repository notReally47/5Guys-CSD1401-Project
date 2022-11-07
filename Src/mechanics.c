#include "cprocessing.h"
#include "structs.h"        // rect
#include "defines.h"        // colours, rows/cols
#include "utils.h"          // IsAreaClicked(), extern duration/lockTimer
#include "level_logic.h"     // extern level
#include "basegame.h"       // for base game state
#include "mechanics.h"      // to call function

extern Config config;
extern Customer customer[CUSTOMER];
extern Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];

rect card,text;
float size1,size2;
int cards[MAX_CARDS],size,pos,pos2,infected[CUSTOMER+1],tele[UM];

char* negcards[] = {
    "Contagious","Some of the customers are infectious! Talking to them will spread its germs to you, causing you to periodically stand in place and let out a cough",
    "Leakage","Oh no! Puddles of water has formed due to faulty air conditioning on the ceiling. Wet floor signs has been put up around the store, blocking certain areas",
    "Holiday sales","It's that time of the year for festivities. Expect more customers pouring in, as they enjoy discounted prices",
    "Blabbermouth","You love chatting up with the customers, however you tend to get lost in time, as you spend more time talking than working while on the clock",
    "Bulk order","Sales have been doing well, meaning shelves are emptying out regularly. That also means more packages that you have to stock up for the day",
    "Half shift","You're off the clock much earlier than usual, but you still have the same amount of work that your boss expects you to get done. Capitalism at its finest"
};
char* poscards[] = {
    "Overtime","Extended closing hours is a bane. It could also be a blessing as you have a longer period to get your work done",
    "It's a slow day","People rather stay at home than going outside, but you wouldn't care. You're just here for the paycheck! Lesser patrons wander the store",
    "Upgrade!","Your boss invested in technology to aid his employees. Receive a pair of teleporting pads for instantaneous travel across the store",
    "Intuitive","You read the customers like a book! Lower the duration the customer is engaged with you, as you answer their questions before they could even ask",
    "Abundant supplies","The shelves are well-stocked before you start your shift. Lucky for you, that means lesser packages that you have to unpack for the day",
    "Elusive","Your display of work ethic deters customers from approaching you as they see that you're busy. Well, so long as you're by a package that is"
};

void card_init(void) {
    /*Settings*/
    card.center_x = (float)config.settings.resolutionWidth/3.f;
    card.center_y = (float)config.settings.resolutionHeight*0.5f;
    card.width = (float)config.settings.resolutionWidth/6.f;
    card.height = (float)config.settings.resolutionHeight/3.f;
    text.center_x = card.center_x-card.width/2.4f;
    text.center_y = card.center_y-card.height*0.5f;
    text.width = (float)config.settings.resolutionWidth/7.f;
    text.height = card.height*0.2f;
    size1 = (float)config.settings.resolutionHeight*0.04f;
    size2 = (float)config.settings.resolutionHeight*0.025f;
    
    /*Initialization*/
    size = 5;
    for (int i=0;i<UM;i++)
        tele[i] = 0;
    for (int i=0;i<MAX_CARDS;i++) // set array to 0 (called cause last value has to be 0 as a buffer)
        cards[i] = 0;
    for (int i=0;i<=size;i++) // set all but last value to the card number
        cards[i] = 2*i;
    for (int i=0;i<=CUSTOMER;i++)
        infected[i] = 0;
    pos = CP_Random_RangeInt(0,size); 
    do {
        pos2 = CP_Random_RangeInt(0,size);
    } while (pos2==pos);
}
void card_deck(int* POS,int cards[], int* size) {
    if (*size != 1){
        for (int i=*POS;i<=*size;i++)
            cards[i] = cards[i+1];
        *POS = CP_Random_RangeInt(0,--*size);
        if (POS == &pos){
            do {
                pos2 = CP_Random_RangeInt(0,*size);
            } while (pos2 == *POS);
        }
        else if (POS == &pos2){
            do {
                pos = CP_Random_RangeInt(0,*size);
            } while (pos == *POS);
        }
    }
}
void card_selection(int stage, CP_BOOL CP_Input_MouseClicked) {
    char* pickcard[12] = {'\0'};
    for (int i=0;i<12;i++)
        pickcard[i] = (stage == 1) ? negcards[i] : poscards[i];
    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Settings_Fill(BLUEGRAY);
    CP_Graphics_DrawRect(card.center_x,card.center_y,card.width,card.height);                                       // Card selection 1
    CP_Graphics_DrawRect(card.center_x+card.center_x,card.center_y,card.width,card.height);                         // Card selection 2
    CP_Settings_Fill(BLACK);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);
    CP_Settings_TextSize(size1);
    CP_Font_DrawTextBox(pickcard[cards[pos]],text.center_x,text.center_y+text.height,text.width);                   // Card selection 1
    CP_Font_DrawTextBox(pickcard[cards[pos2]],text.center_x+card.center_x,text.center_y+text.height,text.width);    // Card selection 2
    CP_Settings_TextSize(size2);
    CP_Font_DrawTextBox(pickcard[cards[pos]+1],text.center_x,text.center_y+2*text.height,text.width);               // Card selection 1
    CP_Font_DrawTextBox(pickcard[cards[pos2]+1],text.center_x+card.center_x,text.center_y+2*text.height,text.width);// Card selection 2

    if (CP_Input_MouseClicked) {
        if (IsAreaClicked(card.center_x, card.center_y, card.width, card.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            card_effect(pos, cards);
            card_deck(&pos, cards, &size);
        }
        else if (IsAreaClicked(card.center_x + card.center_x, card.center_y, card.width, card.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            card_effect(pos2, cards);
            card_deck(&pos2, cards, &size);
        }
        CP_Engine_SetNextGameState(base_Init,base_Update,base_Exit); // Go to the Next Level
    }
}
void card_effect(int pos,int cards[]) {
    switch (cards[pos]) {
    case 0: // Card 0 : Contagious || Card 0 : Overtime
        if (global_level==4||global_level==8||global_level==12)
            customer_status(customer);
        else
            duration = 70;
        break;
    case 2: // Card 2 : Leakage || Card 2 : It's a slow day
        if (global_level==4||global_level==8||global_level==12)
            wetsign_UM();
        else
            // customer_number--
        break;
    case 4: // Card 4 : Holiday sales || Card 4 : Upgrade!
        //if (level==4||level==8||level==12)
            // customer_number++
        //else 
            teleport_UM();
        break;
    case 6: // Card 6 : Blabbermouth || Card 6 : Intuitive
        if (global_level==4||global_level==8||global_level==12)
            lockTimer = 4.f;
        else 
            lockTimer = 1.5f;
        break;
    case 8: // Card 8 : Bulk order || Card 8 : Abundant supplies
        //if (level==4||level==8||level==12)
            // number of boxes ++
        //else
            // number of boxes --
        break;
    case 10: // Card 10 : Half shift || Card 10 : Elusive
        if (global_level==4||global_level==8||global_level==12)
            duration = 50;
        else
            // todo
        break;
    }
}
void customer_status(Customer customer[CUSTOMER]) {
    int total = 0;
    for (int i = 0; i < CUSTOMER; i++) 
        if (customer[i].isActive) {
            infected[i] = 1; // to be used in customer.c -> customerLock()
            total++;
            if (total == 4)
                break;
        }
}
int player_status(int* isLocked) {
    if (infected[10] == 1){
    static float elapse = 0.f;
    elapse += CP_System_GetDt();
        if (elapse >= 2.f)
            *isLocked = (CP_Random_RangeInt(0,10)%10==0)?1:0; // 10% chance of getting locked every 2 seconds 
    }
    return *isLocked;
}
void teleport_UM(void) {
    // initialize teleport_UM array
    tele[0] = 1; // enable tele
    tele[1] = 4; // row of 1st
    tele[2] = 4; // col of 1st
    tele[3] = 10; // row of 2nd
    tele[4] = 10; // col of 2nd
    tele[5] = 0; // cooldown
}
void wetsign_UM(void) {
    int total = 0;
    for (int row = 1; row < SOKOBAN_GRID_ROWS-1; row++){
        for (int col = 1; col < SOKOBAN_GRID_COLS-1; col++){
            int counter = 0;
            for (int neighbour=0;neighbour<9;neighbour++) { // create a 5x5 grid //, nrow2 = row+((neighbour%5)-2), ncol2=col+((neighbour/5)-2)
                int nrow = row+((neighbour%3)-1), ncol=col+((neighbour/3)-1); // nrow/ncol is the updated row/col by adding -1,0,1. this is formed by using %3-1 and /3-1
                if (grid[nrow][ncol].player || grid[nrow][ncol].box || grid[nrow][ncol].shelf || grid[nrow][ncol].key || grid[nrow][ncol].boarder || grid[nrow][ncol].customer || grid[nrow][ncol].mecha)
                    counter += (nrow>=0 && nrow<SOKOBAN_GRID_ROWS && ncol>=0 && ncol<SOKOBAN_GRID_COLS) ? 1 : 0; // filter out-of-bounds array, and add the grid value
            }
            if (counter == 0) {
                grid[row][col].mecha = 1;
                total++; // should be 169 at level 1
            }
        }
    }
    for (int row = 2; row < SOKOBAN_GRID_ROWS-2; row++) 
        for (int col = 2; col < SOKOBAN_GRID_COLS-2; col++) 
            if (grid[row][col].mecha == 1 && CP_Random_GetBool()==YES && total>100) { // 50% chance of removing, to be more sporadic
                grid[row][col].mecha = 0;
                total--;
            }
    for (int row = SOKOBAN_GRID_ROWS-2; row > 2; row--) 
        for (int col = SOKOBAN_GRID_COLS-2; col > 2; col--) 
            if (grid[row][col].mecha == 1 && CP_Random_GetBool()==YES && total>50) { // 50% chance of removing, to be more sporadic
                grid[row][col].mecha = 0;
                total--;
            }
    for (int row = 2; row < SOKOBAN_GRID_ROWS-2; row++) 
        for (int col = 2; col < SOKOBAN_GRID_COLS-2; col++) 
            if (grid[row][col].mecha == 1 && CP_Random_GetBool()==YES && total>30) { // 50% chance of removing, to be more sporadic
                grid[row][col].mecha = 0;
                total--;
            }
    for (int row = SOKOBAN_GRID_ROWS-2; row > 2; row--) 
        for (int col = SOKOBAN_GRID_COLS-2; col > 2; col--) 
            if (grid[row][col].mecha == 1 && CP_Random_GetBool()==YES && total>15) { // 50% chance of removing, to be more sporadic
                grid[row][col].mecha = 0;
                total--;
            }
}

