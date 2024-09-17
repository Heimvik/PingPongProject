#include "oledMenu.h"


char* copyString(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy != NULL) {
        strcpy(copy, str);
    }
    return copy;
}

uint16_t findId(const char* str) {
    char* underscorePos = strchr(str, '_');
    if (underscorePos == NULL) {
        printf("Invalid page or item id in string: %s",str);
    }
    underscorePos++;
    uint16_t number = 0;
    while (*underscorePos != '\0') {
        if (*underscorePos >= '0' && *underscorePos <= '9') {
            number = number * 10 + (*underscorePos - '0');
        }
        underscorePos++;
    }
    return number;
}

//Init the page and its items non-relational
Page_t* initPage(PageInfo_t pageInfo) {
    Page_t* page = (Page_t*)malloc(sizeof(Page_t));
    PageItem_t* pageItems = (PageItem_t*)malloc(sizeof(PageItem_t) * pageInfo.numItems);

    page->name = pageInfo.name;
    page->numItems = pageInfo.numItems;
    page->items = pageItems;
    page->overPage = NULL;

    for(uint8_t i =0;i<page->numItems;i++){
        page->items[i].text = (char*)malloc(sizeof(char)*strlen(pageInfo.items[i]));
        strcpy(page->items[i].text,pageInfo.items[i]);
        page->items[i].page = NULL;
    }

    return &page;
}

//Takes in a arbitrary page ptr, finds the top page, and:
// - Relates all underpages to the overpage
// - Relates all items in a overpage with its menupage
Page_t** relatePages(Page_t** pages, uint8_t numPages){
    //Find the top page
    Page_t topPage;
    for(uint8_t i = 0;i<numPages;i++){
        if (pages[i]->overPage == NULL){
            topPage = *pages[i];
            break;
        }
    }

    for(size_t i = 0; i<numPages;i++){
        for(size_t j=0; j<pages[i]->numItems;j++){
            for(size_t k=0; k<numPages;k++){
                if(k == i){
                    //A page is assumed to not contain a item to itself
                    continue;
                }
                uint16_t itemId = findId(pages[i]->items[j].text);
                uint16_t pageId = findId(pages[k]->name);
                if(itemId == pageId){
                    pages[k]->overPage = pages[i];
                    pages[i]->items[j].page = pages[k];
                    break;
                }
            }
        }
    }
}

Menu_t initMenu(){
    /*
        Definition of the page content, the initialization favors the following strucutre:

        {PAGE-NAME_ID, NUM_ITEMS, {ITEM-NAME_ID, ITEM-NAME_ID, ITEM-NAME_ID, ITEM-NAME_ID}, OVERPAGE-NAME_ID}       
    
        The IDs wil be used for setting upp the conections between all pages, to they have to be correct.
    */
    PageInfo_t pageInfo[] = {
        {"PAGE_0",4,{"ITEM_01", "ITEM_02","ITEM_03","ITEM_04"}, NULL},
        {"PAGE_01",3, {"ITEM_010", "ITEM_011", "ITEM_012"}, "PAGE_0"},
        {"PAGE_02",3, {"ITEM_020", "ITEM_020", "ITEM_020"}, "PAGE_0"},
        {"PAGE_03",3, {"ITEM_030", "ITEM_030", "ITEM_030"}, "PAGE_0"},
        {"PAGE_04",3, {"ITEM_040", "ITEM_040", "ITEM_040"}, "PAGE_0"}    
    };

    Page_t** pages = (Page_t**)malloc(sizeof(Page_t*)*NUM_PAGES);
    for(int i = 0;i<NUM_PAGES;i++){
        pages[i] = initPage(pageInfo[i]);
    }

    relatePages(pages,(uint8_t)NUM_PAGES);
    
    //Init menu 
    Menu_t menu = {pages[0]};
    return menu;
}




void menuSelect() {
    PageItem_t mainMenuItems[] = {
        {"Play", NULL},
        {"Settings", NULL},
        {"Highscore", NULL},
        {"Exit", NULL},
        {"PLZ GODKJENN", NULL}
    };

    Page_t mainMenu = {
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


void menuPrint(Page_t menuPage, uint8_t menuIndex){
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
