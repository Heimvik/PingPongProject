#include "gameLogic.h"

uint16_t goals = 0;
uint64_t lastGoal = 0;
uint16_t goaling=0;

uint16_t checkAndReturnGoals()
{
    if (readAdc() < 1069)
    {
        if (!goaling)
        {
            goaling = 1;
            goals++;
        }
    }
    else
    {
        goaling = 0;
    }
    return goals;
}
