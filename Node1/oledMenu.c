#include "oledMenu.h"


Page_t* findTopPage(Page_t* currentPage){
    Page_t* topPage = currentPage;
    while(topPage->overPage != NULL){
        topPage = currentPage->overPage;
    }
    return topPage;
}

Page_t* findDeepestPage(Page_t* page, uint8_t currentDepth, uint8_t* maxDepth) {
    Page_t* deepestPage = page;
    *maxDepth = currentDepth;

    for (int i = 0; i < page->numItems; i++) {
        if (page->items[i].page != NULL) {
            uint8_t childDepth;
            Page_t* childDeepest = findDeepestPage(page->items[i].page, currentDepth + 1, &childDepth);

            if (childDepth > *maxDepth) {
                *maxDepth = childDepth;
                deepestPage = childDeepest;
            }
        }
    }
    return deepestPage;
}

char* findId(const char* str) {
    char* underscorePos = strchr(str, '_');
    if (underscorePos == NULL) {
        printf("Invalid page or item id in string: %s\n", str);
        return NULL;
    }

    underscorePos++;
    size_t len = 0;
    char* tempPos = underscorePos;
    while (*tempPos != '\0') {
        if (*tempPos < '0' || *tempPos > '9') {
            printf("Invalid character in ID: %s\n", str);
            return NULL;
        }
        len++;
        tempPos++;
    }
    char* id = (char*)malloc(len + 1);
    strncpy(id, underscorePos, len);
    id[len] = '\0'; 
    return id;
}

//Init the page and its items non-relational
Page_t* initPage(PageInfo_t pageInfo) {
    Page_t* page = (Page_t*)malloc(sizeof(Page_t));
    PageItem_t* pageItems = (PageItem_t*)malloc(sizeof(PageItem_t) * pageInfo.numItems);
    char* pageName = (char*)malloc(strlen(pageInfo.name) + 1);
    strcpy(pageName, pageInfo.name);
    page->name = pageName;
    page->numItems = pageInfo.numItems;
    page->items = pageItems;
    page->overPage = NULL;

    for(uint8_t i =0;i<page->numItems;i++){
        page->items[i].text = (char*)malloc(sizeof(char)*strlen(pageInfo.items[i]));
        strcpy(page->items[i].text,pageInfo.items[i]);
        page->items[i].page = NULL;
    }

    return page;
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
                char* itemId = findId(pages[i]->items[j].text);
                char* pageId = findId(pages[k]->name);
                if(strcmp(itemId,pageId)==0){
                    pages[k]->overPage = pages[i];
                    pages[i]->items[j].page = pages[k];
                    free(itemId);
                    free(pageId);
                    break;
                }
                free(itemId);
                free(pageId);
            }
        }
    }
}

Menu_t* initMenu(){
    /*
        Definition of the page content, the initialization favors the following strucutre:

        {PAGE-NAME_ID, NUM_ITEMS, {ITEM-NAME_ID, ITEM-NAME_ID, ITEM-NAME_ID, ITEM-NAME_ID}, OVERPAGE-NAME_ID}       
    
        The IDs wil be used for setting upp the conections between all pages, to they have to be correct.
    */
    PageInfo_t pageInfo[] = {
        {"PAGE_0",4,{"ITEM_00", "ITEM_01","ITEM_02","ITEM_03"}, NULL},
        {"PAGE_00",3, {"ITEM_000", "ITEM_001", "ITEM_002"}, "PAGE_0"},
        {"PAGE_000",3, {"ITEM_0000", "ITEM_0001", "ITEM_0002"}, "PAGE_00"},
        {"PAGE_0000",3, {"ITEM_00000", "ITEM_00001", "ITEM_00002"}, "PAGE_000"},
        {"PAGE_001",3, {"ITEM_0010", "ITEM_0011", "ITEM_0012"}, "PAGE_00"},
        {"PAGE_01",3, {"ITEM_010", "ITEM_011", "ITEM_012"}, "PAGE_0"},
        {"PAGE_02",3, {"ITEM_020", "ITEM_021", "ITEM_022"}, "PAGE_0"},
        {"PAGE_03",3, {"ITEM_030", "ITEM_031", "ITEM_032"}, "PAGE_0"}   
    };

    Page_t** pages = (Page_t**)malloc(sizeof(Page_t*)*NUM_PAGES);
    for(int i = 0;i<NUM_PAGES;i++){
        pages[i] = initPage(pageInfo[i]);
    }

    relatePages(pages,(uint8_t)NUM_PAGES);
    
    Menu_t* menu = (Menu_t*)malloc(sizeof(Menu_t));
    menu->currentPage = pages[0];
    free(pages);
    return menu;
}

void displayPage(Page_t* page) {
    printf("\nPage: %s\n", page->name);
    for (int i = 0; i < page->numItems; i++) {
        printf("%d: %s\n", i, page->items[i].text);
    }
    printf("\n");
}

void navigateMenu(Menu_t* menu) {
    uint8_t maxDepth = 0;
    printf("The deepest page is: %s\n", findDeepestPage(menu->currentPage, 0, &maxDepth)->name);

    int choice;
    while (1) {
        displayPage(menu->currentPage);
        printf("Choose an item (0 to %d), -2 to quit or -1 to go up: ", menu->currentPage->numItems - 1);
        scanf("%d", &choice);

        if (choice == -1) {
            if (menu->currentPage->overPage) {
                menu->currentPage = menu->currentPage->overPage;
            } else {
                printf("No parent page to go back to.\n");
            }
        } if (choice == -2) {
            return;
        } else if (choice >= 0 && choice < menu->currentPage->numItems) {
            Page_t* nextPage = menu->currentPage->items[choice].page;
            if (nextPage) {
                printf("Choice: %s\n", menu->currentPage->items[choice].text);
                menu->currentPage = nextPage;
            } else {
                printf("Invalid choice or no linked page.\n");
            }
        } else {
            printf("Invalid input.\n");
        }
    }
}

void freeMenu(Menu_t* menu) {
    Page_t* currentPage = findTopPage(menu->currentPage);
    uint8_t pagesFreed = 0;

    while (pagesFreed != NUM_PAGES) {

        uint8_t currentDepth = 0;
        currentPage = findDeepestPage(currentPage, 0, &currentDepth);

        for (uint8_t i = 0; i < currentPage->numItems; i++) {
            free(currentPage->items[i].text);
        }
        free(currentPage->items);
        printf("Freed page: %s\n", currentPage->name);
        free(currentPage->name);

        if(currentPage->overPage != NULL){
            Page_t* tempPage = currentPage;
            currentPage = currentPage->overPage;
            for(uint8_t i = 0; i < currentPage->numItems; i++){
                if(currentPage->items[i].page == tempPage){
                    currentPage->items[i].page = NULL;
                    break;
                }
            }
            free(tempPage);
            currentPage = findTopPage(menu->currentPage);
        } else {
            free(currentPage);
        }
        pagesFreed++;
        printf("Freed top page, total pages freed vs. expected: %d | %d \n", pagesFreed, NUM_PAGES);
    }
    printf("Menu freed\n");
    free(menu);
}


//Old functionality
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
