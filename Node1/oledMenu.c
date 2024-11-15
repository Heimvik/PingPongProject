#include "oledMenu.h"

void menuPrint(MenuPage_t menuPage, uint8_t menuIndex);

MenuPage_t initMenuPage(char* name){
    
}

Menu_t initMenu(char* name, uint8_t numItems){
    
}



uint8_t menuSelect() {
    MenuItem_t mainMenuItems[] = {
        {"Play", NULL},
        {"Exit", NULL},
        {"PLZ GODKJENN", NULL}
    };

    MenuPage_t mainMenu = {
        "Main menu",
        3,
        mainMenuItems
    };

    uint8_t menuIndex = 0;
    uint8_t debounceCounter = 0;
    const uint8_t debounceThreshold = 50; // Adjust this value as needed
    joyDirection_t lastDirection = NEUTRAL; // Assuming NEUTRAL is a valid direction

    while (1) {
        
        joyDirection_t dir = ReadADC().joyDirection;
        uint8_t sliderLeft = ReadADC().sliderLeft;
        if (dir == RIGHT) {
            return 1;
        }
        if (dir == lastDirection) {
            debounceCounter++;
        } else {
            debounceCounter = 0;
        }

        if (debounceCounter >= debounceThreshold) {
            if (dir == DOWN) {
                menuIndex = (menuIndex + 1) % mainMenu.numItems;
            } else if (dir == UP) {
                menuIndex = (menuIndex - 1 + 3) % mainMenu.numItems; // Ensure menuIndex stays positive
            }
            debounceCounter = 0; // Reset the counter after registering the input
        }

        lastDirection = dir;
        menuPrint(mainMenu, menuIndex);
    }
}


void menuPrint(MenuPage_t menuPage, uint8_t menuIndex){
    for (int i = 0; i < menuPage.numItems; ++i){
        if (i == menuIndex){
            OledPrintLnInverted(i, menuPage.items[i].text);
        }
        else{
        OledPrintLn(i, menuPage.items[i].text);
        }
    }

    OledWriteOutFb();
}


void displayGoals(uint8_t goals){
    OledReset();
    char goalString[16];
    sprintf(goalString, "Goals: %d", goals);
    OledPrintLn(0, goalString);
    OledWriteOutFb();
}