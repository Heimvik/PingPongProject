#pragma once

#include "drivers/oled.h"
#include "drivers/adc.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define _LEVELS 2
#define NUM_PAGES 8
#define NUM_ITEMS 8


//For definfing the variables in the 
typedef struct {
    char* name;
    uint8_t numItems;
    char* items[NUM_ITEMS];
    char* overPage;
} PageInfo_t;

typedef struct {
    char* text;
} ItemInfo_t;


const extern PageInfo_t pageInfo[];


//For the actual menu

uint8_t findNumItems(char* page);

typedef struct Page_t Page_t;
typedef struct PageItem_t PageItem_t;

typedef struct PageItem_t {
    char* text;
    Page_t* page; 
} PageItem_t;

typedef struct Page_t {
    char* name;
    uint8_t numItems;
    PageItem_t* items; 
    Page_t* overPage; 
} Page_t;

typedef struct {
    Page_t* currentPage;
} Menu_t;

Page_t* initPage(PageInfo_t pageInfo);
Page_t** relatePages(Page_t** pages, uint8_t numPages);

Menu_t* initMenu();
void navigateMenu(Menu_t* menu);
void freeMenu(Menu_t* menu);
