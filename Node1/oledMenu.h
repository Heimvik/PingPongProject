#pragma once
#include "drivers/oled.h"
#include "drivers/adc.h"


typedef struct {
    char* text;
} MenuItem_t;

typedef struct {
    char* name;
    uint8_t numItems;
    MenuItem_t* items;    
} MenuPage_t;

typedef struct {
    MenuPage_t* currentPage;
    MenuPage_t* rootPage;
} Menu_t;

Menu_t initMenu(char* name,uint8_t numItems);
MenuPage_t initMenuPage(char* name);

uint8_t menuSelect();

void displayGoals(uint8_t goals);
