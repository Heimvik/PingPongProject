#include "oledMenu.h"


char* copyString(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy != NULL) {
        strcpy(copy, str);
    }
    return copy;
}


void initMenuPage(MenuPage_t* pages, size_t pageCount){
    for( size_t i = 0; i<pageCount;i++){

        //Init to what we defined in menuPages
        pages[i].name = copyString(menuPages[i].name);
        pages[i].numItems = menuPages[i].numItems;
        pages[i].items = (MenuItem_t*)malloc(sizeof(MenuItem_t) * pages[i].numItems);

        for (size_t j = 0; j < pages[i].numItems; j++) {
            pages[i].items[j].text = copyString(menuPages[i].items[j]);
            pages[i].items[j].page = NULL;
        }

        if (menuPages[i].overPage != NULL) {
            for (size_t k = 0; k < pageCount; k++) {
                if (strcmp(menuPages[i].overPage, pages[k].name) == 0) {
                    pages[i].overPage = &pages[k];
                    break;
                }
            }
        }
    }
}

void initMenuItems(MenuPage_t* pages, size_t pageCount, MenuItem_t* items, size_t itemCount) {
    for(size_t i = 0; i<pageCount;i++){
        for(size_t j=0; j<itemCount;j++){
            for(size_t k=0; k<pageCount;k++){
                if(strcmp(pages[i].items[j].text,pages[k].name)==0){
                    pages[i].items[j].page = &pages[k];
                } else {
                    continue;
                }
            }
        }
    }
}

void freeMenuPages(MenuPage_t* pages, size_t pageCount) {
    for (size_t i = 0; i < pageCount; i++) {
        free(pages[i].name);
        for (size_t j = 0; j < pages[i].numItems; j++) {
            free(pages[i].items[j].text);
        }
        free(pages[i].items);
    }
}

void freeMenuItems(MenuItem_t* items, size_t itemCount) {
    for (size_t i = 0; i < itemCount; i++) {
        free(items[i].text);
    }
}
Menu_t initMenu(){
    size_t pageCount = sizeof(menuPages) / sizeof(menuPages[0]);
    size_t itemCount = sizeof(menuItems) / sizeof(menuItems[0]);

    MenuPage_t* pages = (MenuPage_t*)malloc(sizeof(MenuPage_t) * pageCount);
    MenuItem_t* items = (MenuItem_t*)malloc(sizeof(MenuItem_t) * itemCount);

    initializeMenuPages(pages, pageCount);
    initializeMenuItems(items, itemCount, pages, pageCount);

    Menu_t menu = {pages};
    return menu;    
}

void freeMenu(Menu_t* menu){
    //Set the menu page to be the top page
    freeMenuPages(menu.currentpage, pageCount);
    freeMenuItems(items, itemCount);
    free(pages);
    free(items);

}



/*
void menuSelect() {
    MenuItem_t mainMenuItems[] = {
        {"Play", NULL},
        {"Settings", NULL},
        {"Highscore", NULL},
        {"Exit", NULL},
        {"PLZ GODKJENN", NULL}
    };

    MenuPage_t mainMenu = {
        "Main menu",
        5,
        mainMenuItems
    };

    uint8_t menuIndex = 0;
    uint8_t debounceCounter = 0;
    const uint8_t debounceThreshold = 50; // Adjust this value as needed
    joyDirection_t lastDirection = NEUTRAL; // Assuming NEUTRAL is a valid direction

    while (1) {
        joyDirection_t dir = ReadADC().joyDirection;
        uint8_t sliderLeft = ReadADC().sliderLeft;
        if (sliderLeft > 200) {
            printf("Selected item: %s\n", mainMenu.items[menuIndex].text);
        }
        if (dir == lastDirection) {
            debounceCounter++;
        } else {
            debounceCounter = 0;
        }

        if (debounceCounter >= debounceThreshold) {
            if (dir == UP) {
                menuIndex = (menuIndex + 1) % mainMenu.numItems;
            } else if (dir == DOWN) {
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
*/