#include "gameLogic.h"

uint16_t goals = 0;
uint64_t lastGoal = 0;
uint16_t goaling=0;

#define GOAL_THRESHOLD 1069
#define DEBOUNCE_COUNT 20

uint16_t checkAndReturnGoals()
{
    static uint8_t debounceCounter = 0;

    if (readAdc() < GOAL_THRESHOLD)
    {
        if (debounceCounter < DEBOUNCE_COUNT)
        {
            debounceCounter++;
        }
        else if (!goaling)
        {
            goaling = 1;
            goals++;
        }
    }
    else
    {
        debounceCounter = 0;
        goaling = 0;
    }
    return goals;
}