/*
All content ? 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Shafiq Mirza Bin Mohamed Zahid (shafiqmirza.b@digipen.edu)
          Jerell Tan Jian Yu (jerelljianyu.tan@digipen.edu)
            - Clicking SFX
            - Level Complete SFX
          Guo Yiming (yiming.guo@digipen.edu)
            - Tinted Buttons
            - Card selection confirmation flag
File	: mechanics.c
Purpose	: For our unique mechanics card deck system that provides various positive/negative effects each level
*/

#include "cprocessing.h"    // CP_Random_RangeInt(), CP_Random_GetBool()
#include "structs.h"        // rect, UniqueCards, CardPosition, Button, Flag
#include "defines.h"        // CP_Color_Create, SOKOBAN_GRID_COLS/SOKOBAN_GRID_ROWS
#include "utils.h"          // IsAreaClicked(), extern duration/lockTimer
#include "level_logic.h"    // extern level
#include "mechanics.h"      // To call function in mechanics_flag()
#include "basegame.h"       // extern int path[row][col]
#include "level_generate.h" // 
#include "easydraw.h"       // Apply/Discard buttons

extern Config config;                                       // Resolution Width/Height
extern Customer customer[CUSTOMER_MAX];                     // Customer Array for customer_status(), customer generation
extern Cell grid[SOKOBAN_GRID_ROWS][SOKOBAN_GRID_COLS];     // Grid Array for wetsign_UM(), box/key generation
extern int time_lost, ignore_penalty;                       // To increment/decrement value of variables

UniqueCards UM;
CardPosition pos;
Button apply, discard;
rect card,text;
float textsizeheader,textsizedesc;
int infected[CUSTOMER_MAX + 1], teleporter[SETTINGS];       // array that enables certain mechanics if selected
Flag selected = NO;
int cardChosen = 0;
CP_Sound click;

// Initialise char arrays for negative status cards. [even number] card title, [odd number] card description of said title
char* negcards[] = {
    "Contagious","Some of the customers are infectious! Talking to them will spread its germs to you, causing you to periodically stand in place and let out a cough",
    "Leakage","Oh no! Puddles of water has formed due to faulty air conditioning on the ceiling. Wet floor signs has been put up around the store, blocking certain areas",
    "Holiday sales","It's that time of the year for festivities. Expect more customers pouring in, as they enjoy discounted prices",
    "Blabbermouth","You love chatting up with the customers, however you tend to get lost in time, as you spend more time talking than working while on the clock",
    "Bulk order","Sales have been doing well, meaning shelves are emptying out regularly. That also means more packages that you have to stock up for the day",
    "Half shift","You're off the clock much earlier than usual, but you still have the same amount of work that your boss expects you to get done. Capitalism at its finest"
};
// Initialise char arrays for positive status cards. [even number] card title, [odd number] card description of said title
char* poscards[] = {
    "Overtime","Extended closing hours is a bane. It could also be a blessing as you have a longer period to get your work done",
    "It's a slow day","People rather stay at home than going outside, but you wouldn't care. You're just here for the paycheck! Lesser patrons wander the store",
    "Upgrade!","Your boss invested in technology to aid his employees. Receive a pair of teleporting pads for instantaneous travel across the store",
    "Intuitive","You read the customers like a book! Lower the duration the customer is engaged with you, as you answer their questions before they could even ask",
    "Abundant supplies","The shelves are well-stocked before you start your shift. Lucky for you, that means lesser packages that you have to unpack for the day",
    "Elusive","Shrug off your responsibilites once a day. You act as if nothing happened when you get distracted, while avoiding the time penalty"
};
// Initialise settings for displaying of cards in level_select.c
void card_settings(void) {
    /*Settings*/
    card.center_x = (float)config.settings.resolutionWidth/3.f;
    card.center_y = (float)config.settings.resolutionHeight*0.5f;
    card.width = (float)config.settings.resolutionWidth/6.f;
    card.height = (float)config.settings.resolutionHeight/3.f;
    text.center_x = card.center_x-card.width/2.4f;
    text.center_y = card.center_y-card.height*0.5f;
    text.width = (float)config.settings.resolutionWidth/7.f;
    text.height = card.height*0.2f;
    textsizeheader = (float)config.settings.resolutionHeight*0.04f;
    textsizedesc = (float)config.settings.resolutionHeight*0.025f;
}
// Initialise settings for all variables in UniqueCards UM if not saved in config.dat
void card_init(void) {
    if (global_level <= 1) {
    /*Initialization if not in config.dat*/
        UM.flags = 0;                               // set to 0 to reset flag
        UM.selectedflag = 0;                        // set to 0 to reset flag
        UM.negdecksize = 5;                         // initialise decksize back to 5
        UM.posdecksize = 5;                         // as decksize decrements each stage
        for (int i = 0; i <= UM.negdecksize; i++) { // set all but last value to the card number
            UM.negcards[i] = 2 * i;                 // {0,2,4,6,8,10,0}
            if (i == UM.negdecksize)                // the even numbers are the position of char* negcards[]
                UM.negcards[i+1] = 0;               // set last array value as buffer
        }
        for (int j = 0; j <= UM.posdecksize; j++) { // initialised separately in case there's more cards
            UM.poscards[j] = 2 * j;
            if (j == UM.posdecksize)
                UM.poscards[j + 1] = 0;
        }
    }
    else {
        UM = config.UM;
    }
    
    /*Other initialization not dependant on saves*/
    // shuffles left and right cards so the random selection will always be different cards
    pos.neg1 = CP_Random_RangeInt(0, UM.negdecksize);
    do {
        pos.neg2 = CP_Random_RangeInt(0, UM.negdecksize);
    } while (pos.neg2 == pos.neg1);
    // shuffles for positive cards
    pos.pos1 = CP_Random_RangeInt(0, UM.posdecksize);
    do {
        pos.pos2 = CP_Random_RangeInt(0, UM.posdecksize);
    } while (pos.pos2 == pos.pos1);
    // array for infected mechanics
    for (int i = 0; i <= CUSTOMER_MAX; i++)
        infected[i] = 0;
    teleporter[0] = 0;                              // disables teleporter
}
// function that checks for various flags using unsigned integer in bit level
void mechanic_flags(void) {
    /*Unique mechanics Initialisation*/
    // affects time_lost
    if (UM.flags & 64)                              // checks if "Blabbermouth" card is selected
        time_lost += 15;		                    // lose 60 seconds instead when distracted
    if (UM.flags & 128)                             // checks if "Intuitive" card is selected
        time_lost -= 15;		                    // lose 30 seconds instead when distracted
    // affects duration
    if (UM.flags & 2)	                            // checks if "Overtime" card is selected
        duration += 60;                             // gain additional 60 seconds to total timer
    if (UM.flags & 1024)                            // checks if "Half shift" card is selected
        duration -= 60;	                            // lose 60 seconds to total timer
    // affects customer infection status
    if (UM.flags & 1)                               // checks if "Contagious" card is selected
        customer_status(customer);                  // calls customer_status function
    // affects wetsign placement
    if (UM.flags & 4)                               // checks if "Leakage" card is selected
        wetsign_UM();                               // calls wetsign_UM function
    // affect teleport enabler
    if (UM.flags & 32)                              // checks if "Upgrade!" card is selected
        teleport_UM();                              // calls teleport_UM function
    // affects ignore time penalty
    if (UM.flags & 2048)                            // checks if "Elusive" card is selected
        ignore_penalty = 1;                         // overwrites ignore_penalty in basegame.c to 1 to enable
    // affects customer generation
    if (global_level != 1) {
        int CustomerBackup[2][3] = { 0 };							        // removes first 2 customer per stage by default
        for (int i = 0, total = 0; i <= CUSTOMER_MAX; i++) {                // reason for removal is so that, it is easier to add customers that already exists
            if (customer[i].isActive && total < 2) {
                CustomerBackup[total][0] = customer[i].ogCusRow;	        // save original position
                CustomerBackup[total][1] = customer[i].ogCusCol;
                CustomerBackup[total][2] = i;						        // save customer index
                grid[customer[i].cusRow][customer[i].cusCol].customer = 0;  // zeros the grid.customer to disable
                customer[i].cusRow = 0;								        // zeros both row/col to disable
                customer[i].cusCol = 0;
                customer[i].isActive = 0;                                   // zeros customer.isActive flag to disable
                total++;											        // counter to check if total of 2 customers have been disabled
            }
        }
        if (UM.flags & 16) {	                                            // checks if "Holiday Sales" card is selected
            for (int i = 0; i <= CUSTOMER_MAX; i++) {
                if (i == CustomerBackup[0][2]) {					        // restores back the removed customer by re-enabling back to 1
                    customer[i].cusRow = CustomerBackup[0][0];
                    customer[i].cusCol = CustomerBackup[0][1];
                    customer[i].isActive = 1;
                    grid[customer[i].cusRow][customer[i].cusCol].customer = 1;
                }
                else if (i == CustomerBackup[1][2]) {
                    customer[i].cusRow = CustomerBackup[1][0];
                    customer[i].cusCol = CustomerBackup[1][1];
                    customer[i].isActive = 1;
                    grid[customer[i].cusRow][customer[i].cusCol].customer = 1;
                }
            }
        }
        if (UM.flags & 8) {	                                                            // checks if "It's a slow day" card is selected
            int customer_number[2] = { 0 };
            for (int i = 0, total = 0; i <= CUSTOMER_MAX; i++) {	                    // removes 2 more customers
                if (customer[i].isActive && CP_Random_GetBool() == YES && total < 2) {  // CP_Random_GetBool() to randomly disable customer index
                    grid[customer[i].cusRow][customer[i].cusCol].customer = 0;
                    customer[i].cusRow = 0;
                    customer[i].cusCol = 0;
                    customer[i].isActive = 0;
                    customer_number[total] = i;
                    total++;
                }
            }
        }
    }
    // affect box generation
    if ((global_level != 1) && (original_box_count > 2) && (global_level != 10)) {
        int BoxBackup[2][3] = { 0 };
        int KeyBackup[2][3] = { 0 };
        for (int row = 0, totalbox = 0, totalkey = 0; row < SOKOBAN_GRID_ROWS; row++)
            for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {                 // left function as is, because default removal used to be random
                if (grid[row][col].box == 1 || grid[row][col].box == 2) {       // removes first 2 boxes per stage by default
                    BoxBackup[totalbox][0] = row;						        // save box row position
                    BoxBackup[totalbox][1] = col;
                    BoxBackup[totalbox][2] = grid[row][col].box;		        // save box id that's being removed
                    grid[row][col].box = 0;								        // zeros the box to disable
                    totalbox++;											        // counter to check if total of 2 boxes have been disabled
                }
                else if (grid[row][col].key == 1 || grid[row][col].key == 2) {  // removes first 2 keyes per stage by default
                    KeyBackup[totalkey][0] = row;
                    KeyBackup[totalkey][1] = col;
                    KeyBackup[totalkey][2] = grid[row][col].key;		        // save key id that's being removed
                    grid[row][col].key = 0;								        // zeros the key to disable
                    totalkey++;											        // counter to check if total of 2 keyes have been disabled
                }
            }

        if (UM.flags & 256)                                                     // checks if "Bulk order" card is selected
            for (int row = 0; row < SOKOBAN_GRID_ROWS; row++)                   // restores back boxes/keyes
                for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
                    if (row == BoxBackup[0][0] && col == BoxBackup[0][1])		// Box[0][0] -> row, Box[0][1] -> col of first box
                        grid[row][col].box = BoxBackup[0][2];					// Restore Box ID from Box[0][2]
                    else if (row == BoxBackup[1][0] && col == BoxBackup[1][1])
                        grid[row][col].box = BoxBackup[1][2];
                    else if (row == KeyBackup[0][0] && col == KeyBackup[0][1])
                        grid[row][col].key = KeyBackup[0][2];
                    else if (row == KeyBackup[1][0] && col == KeyBackup[1][1])
                        grid[row][col].key = KeyBackup[1][2];
                }

        if (UM.flags & 512)                                                                     // checks if "Abundant supplies" card is selected
            for (int row = 0, totalbox = 0, totalkey = 0; row < SOKOBAN_GRID_ROWS; row++)
                for (int col = 0; col < SOKOBAN_GRID_COLS; col++) {
                    if (grid[row][col].box && CP_Random_GetBool() == YES && totalbox < 2) {     // randomly removes 2 more boxes using CP_Random_GetBool()
                        grid[row][col].box = 0;
                        totalbox++;
                    }
                    else if (grid[row][col].key && CP_Random_GetBool() == YES && totalkey < 2) {// randomly removes 2 more keyes using CP_Random_GetBool()
                        grid[row][col].key = 0;
                        totalkey++;
                    }
                }
    }
}
/* card_deck shifts the cards[] based on the position selected and reduce the deck size for CP_Random_RangeInt(0,*size) call
* eg, if cards[] = {0,2,4,6,8,10,0} and *selectedpos = 3
* cards[3] = 6
* cards[] = {0,2,4,8,10,0,0} // removes 6 from the deck and shifts over 8 and 10
* *selectedpos now randomly calls 0 to 4 (down from initial 5 deck size)
*/
void card_deck(int* selectedpos, int* notselectedpos, int cards[], int* size) {
    if (*size != 1){
        for (int i=*selectedpos;i<=*size;i++)
            cards[i] = cards[i+1];                          // copies the value of the next int in array (reason for the 0 as the buffer)
        *selectedpos = CP_Random_RangeInt(0,--*size);       // re-assigns *selectedpos randomly with reduced deck size
        do {
            *notselectedpos = CP_Random_RangeInt(0,*size);
        } while (*notselectedpos == *selectedpos);          // keeps re-shuffling until both positions are different
    }
    for (int i = 2; i < 10; i++) {
        if (global_level == i)
            UM.selectedflag |= (2 << (i - 2));
    }
}
// card_selection displays 2 cards during the level complete transition screen that loads either negcards[] or poscards[] based on the level
void card_selection(int stage, int *applied) {
    /*Initialise Apply and Discard Buttons*/
    CP_Vector window = CP_Vector_Set((float)CP_System_GetWindowWidth(), (float)(CP_System_GetWindowHeight()));
    float imgSize = window.y / 20.f;
    setButton(&apply, "./Assets/UI/Apply.png", window.x / 2.f - .75f * imgSize, window.y - (window.y / 5.f), imgSize, imgSize, YES);
    setButton(&discard, "./Assets/UI/Discard.png", window.x / 2.f + .75f * imgSize, window.y - (window.y / 5.f), imgSize, imgSize, YES);

    if (*applied == 0) {   // flag that removes card display on selection
        char* pickcard[12] = { '\0' };  // pickcard/cardpos1/cardpos2 are variables that gets assigned negative/postive based on level
        for (int i = 0; i < 12; i++)    // stage is a flag that is either 1 or 0 when called in level_transition.c
            pickcard[i] = (stage == 0) ? negcards[i] : poscards[i]; // if stage == 1, negcards is selected for level 2,4,6,8
        int* cardpos1 = (stage == 0) ? &pos.neg1 : &pos.pos1;
        int* cardpos2 = (stage == 0) ? &pos.neg2 : &pos.pos2;
        int* decksize = (stage == 0) ? &UM.negdecksize : &UM.posdecksize;
        int* carddeck = (stage == 0) ? UM.negcards : UM.poscards;

        CP_Settings_StrokeWeight(0.0f);
        if (cardChosen == 1) {
            CP_Settings_Fill(NEON_PINK);
            CP_Graphics_DrawRect(card.center_x, card.center_y, card.width + 0.01f * (float)CP_System_GetWindowWidth(), card.height + 0.01f * (float)CP_System_GetWindowWidth());
        }
        else if (cardChosen == 2) {
            CP_Settings_Fill(NEON_PINK);
            CP_Graphics_DrawRect(card.center_x + card.center_x, card.center_y, card.width + 0.01f * (float)CP_System_GetWindowWidth(), card.height + 0.01f * (float)CP_System_GetWindowWidth());
        }

        /*Rendering of cards*/
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Settings_Fill(BLUEGRAY);
        CP_Graphics_DrawRect(card.center_x, card.center_y, card.width, card.height);                                                        // Left card selection
        CP_Graphics_DrawRect(card.center_x + card.center_x, card.center_y, card.width, card.height);                                        // Right card selection
        CP_Settings_Fill(BLACK);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);
        CP_Settings_TextSize(textsizeheader);
        CP_Font_DrawTextBox(pickcard[carddeck[*cardpos1]], text.center_x, text.center_y + text.height, text.width);                         // Left card selection
        CP_Font_DrawTextBox(pickcard[carddeck[*cardpos2]], text.center_x + card.center_x, text.center_y + text.height, text.width);         // Right card selection
        CP_Settings_TextSize(textsizedesc);
        CP_Font_DrawTextBox(pickcard[carddeck[*cardpos1] + 1], text.center_x, text.center_y + 2 * text.height, text.width);                 // Left card selection
        CP_Font_DrawTextBox(pickcard[carddeck[*cardpos2] + 1], text.center_x + card.center_x, text.center_y + 2 * text.height, text.width); // Right card selection
        
        /*Draw Apply and Discard Buttons*/
        if (selected) {
            drawButton(apply);
            drawButton(discard);
        }

        if (CP_Input_MouseClicked()) {
            /*Choose Card No. 1*/
            if (IsAreaClicked(card.center_x, card.center_y, card.width, card.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                selected = YES;
                cardChosen = 1;
                CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
            }
            /*Choose Card No. 2*/
            else if (IsAreaClicked(card.center_x + card.center_x, card.center_y, card.width, card.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                selected = YES;
                cardChosen = 2;
                CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
            }
            if (selected) {
                /*Apply card*/
                if (IsAreaClicked(apply.position.x, apply.position.y, apply.btnWidth, apply.btnHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                    /*Apply card No. 1*/
                    if (cardChosen == 1) {
                        card_effect(*cardpos1, carddeck, stage);
                        card_deck(cardpos1, cardpos2, carddeck, decksize);
                        *applied = YES;
                        selected = NO;
                        cardChosen = 0;
                    }
                    /*Apply card No. 2*/
                    else if (cardChosen == 2) {
                        card_effect(*cardpos2, carddeck, stage);
                        card_deck(cardpos2, cardpos1, carddeck, decksize);
                        *applied = YES;
                        selected = NO;
                        cardChosen = 0;
                    }
                    CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
                }
                /*Discard card*/
                else if (IsAreaClicked(discard.position.x, discard.position.y, discard.btnWidth, discard.btnHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                    selected = NO;
                    cardChosen = 0;
                    CP_Sound_PlayAdvanced(click, 1, 2, FALSE, CP_SOUND_GROUP_SFX);
                }
            }
        }
    }
}
/* card_effect enables the UM.flags that mechanics_flag() checks for. stage is a variable defined in level_select.c,
* where if stage == 1, the card selected is for negative cards. stage is only true for even number levels
*/
void card_effect(int pos,int cards[],int stage) {
    switch (cards[pos]) {
    case 0: // Card 0 : Contagious || Card 0 : Overtime
        UM.flags |= (stage == 1) ? 1 : 2;       // UM.flags & 1     -> customer_status(customer);
        break;                                  // UM.flags & 2     -> duration += 30;
    case 2: // Card 2 : Leakage || Card 2 : It's a slow day
        UM.flags |= (stage == 1) ? 4 : 8;       // UM.flags & 4     -> wetsign_UM();
        break;                                  // UM.flags & 8     -> customer_number--;
    case 4: // Card 4 : Holiday sales || Card 4 : Upgrade!
        UM.flags |= (stage == 1) ? 16 : 32;     // UM.flags & 16    -> customer_number++;
        break;                                  // UM.flags & 32    -> teleport_UM();
    case 6: // Card 6 : Blabbermouth || Card 6 : Intuitive
        UM.flags |= (stage == 1) ? 64 : 128;    // UM.flags & 64    -> time_lost += 15.f;
        break;                                  // UM.flags & 128   -> time_lost -= 15.f;
    case 8: // Card 8 : Bulk order || Card 8 : Abundant supplies
        UM.flags |= (stage == 1) ? 256 : 512;   // UM.flags & 256   -> number of boxes++;
        break;                                  // UM.flags & 512   -> number of boxes--;
    case 10: // Card 10 : Half shift || Card 10 : Elusive
        UM.flags |= (stage == 1) ? 1024 : 2048; // UM.flags & 1024  -> duration -= 30;
        break;                                  // UM.flags & 2048  -> ignore_penalty = 1;
    }
}
// randomly infects customer if 'Contagious' card mechanic is enabled
void customer_status(Customer customer[CUSTOMER_MAX]) {
    int total = 0;
    for (int i = 0; i <= CUSTOMER_MAX; i++)
        infected[i] = 0;                                            // resets to 0 to affect different customers each stage
    for (int i = 0; i < CUSTOMER_MAX; i++) 
        if (customer[i].isActive && CP_Random_GetBool() == YES) {   // chance to infect customers
            infected[i] = 1;                                        // to be used in customer.c -> customerLock()
            total++;
            if (total == 3)                                         // maximum of 3 customers can ever be infected. possibly infect 0 customers even
                break;
        }
}
// if player is infected from 'Contagious' card mechanic, the player has a chance to get locked in place (simulates coughing)
void player_status(int* isLocked) {
    if (infected[10] == 1){                                         // checks if player infection status is true (enabled in customer.c)
        static float elapse = 0.f;
        elapse += CP_System_GetDt();
        if (elapse >= 5.f){
            elapse = 0.f;
            *isLocked = (CP_Random_RangeInt(0,10)%10==0)?2:0;       // 10% chance of getting locked every 5 seconds ?
        }
    }
    //return *isLocked;
}
// enables the teleporter array that affects movement logic in utils.c
void teleport_UM(void) {
    teleporter[0] = 1;      // Teleporter Enabler, 1 is on, 0 is off
    teleporter[1] = 0;      // Teleporter Cooldown
}
// if enabled, randomly place props around the map that blocks certain pathways (possibly to disrupt the flow when moving boxes to objectives)
void wetsign_UM(void) {
    int total = 0;
    for (int row = 2; row < SOKOBAN_GRID_ROWS-1; row++){
        for (int col = 2; col < SOKOBAN_GRID_COLS-1; col++){
            int counter = 0;
            for (int neighbour=0;neighbour<25;neighbour++) { // create a 3x3: nrow = row+((neighbour%3)-1), ncol=col+((neighbour/3)-1) and 5x5 grid: nrow2 = row+((neighbour%5)-2), ncol2=col+((neighbour/5)-2)
                int nrow = row+((neighbour%3)-1), ncol=col+((neighbour/3)-1), nrow2 = row+((neighbour%5)-2), ncol2=col+((neighbour/5)-2); // nrow/ncol is the updated row/col by adding -1,0,1. this is formed by using %3-1 and /3-1
                if (grid[nrow][ncol].player || grid[nrow][ncol].box || grid[nrow][ncol].shelf || grid[nrow][ncol].key || grid[nrow][ncol].boarder || grid[nrow][ncol].customer || grid[nrow][ncol].mecha ||
                    grid[nrow][ncol].tele || grid[nrow2][ncol2].box || grid[nrow2][ncol2].key || path[nrow][ncol])
                    counter += 1;   // counter that increments if any of the conditions is true
            }
            if (counter == 0) {     // only adds a wetsign if counter == 0
                grid[row][col].mecha = 1;
                total++;            // total count of wetsigns in the whole map
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
            }                                                                        // minimum of 15 wetsigns could ever be present
}
// frees all assets used during card_selection
void free_buttons(void) {
    CP_Image_Free(&apply.img);
    CP_Image_Free(&discard.img);
}