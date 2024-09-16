#pragma once
//#include "drivers/oled.h"
//#include "drivers/adc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MENU_LEVELS 2
#define NUM_ITEMS 8

typedef struct {
    char* name;
    uint8_t numItems;
    char* items[NUM_ITEMS];
    char* overPage;
} MenuPageDef;

typedef struct {
    char* text;
    char* pageName; 
} MenuItemDef;

MenuPageDef menuPages[] = {
    {"Main Menu",8,{"Item 0", "Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7"}, NULL},
    {"Sub Menu 1",3, {"Sub Item 0", "Sub Item 1", "Sub Item 2"}, "Main Menu"},
    {"Sub Menu 2",3, {"Sub Item 3", "Sub Item 4", "Sub Item 5"}, "Main Menu"}
};

MenuItemDef menuItems[] = {
    {"Item 0", "Main Menu"},
    {"Item 1", "Main Menu"},
    {"Item 2", "Main Menu"},
    {"Item 3", "Main Menu"},
    {"Item 4", "Main Menu"},
    {"Item 5", "Main Menu"},
    {"Item 6", "Main Menu"},
    {"Item 7", "Main Menu"},
    {"Sub Item 0", "Sub Menu 1"},
    {"Sub Item 1", "Sub Menu 1"},
    {"Sub Item 2", "Sub Menu 1"},
    {"Sub Item 3", "Sub Menu 2"},
    {"Sub Item 4", "Sub Menu 2"},
    {"Sub Item 5", "Sub Menu 2"}
};

uint8_t findNumItems(char* page);


typedef struct MenuPage_t MenuPage_t;
typedef struct MenuItem_t MenuItem_t;

typedef struct MenuItem_t {
    char* text;
    MenuPage_t* page; 
} MenuItem_t;

typedef struct MenuPage_t {
    char* name;
    uint8_t numItems;
    MenuItem_t* items; 
    MenuPage_t* overPage; 
} MenuPage_t;

typedef struct {
    MenuPage_t* currentPage;
} Menu_t;



Menu_t initMenu(char* name,uint8_t numItems);
void initMenuPage(MenuPage_t* overPage, char* id);

void menuSelect();
void menuPrint(MenuPage_t menuPage, uint8_t menuIndex);

Menu_t menu;
