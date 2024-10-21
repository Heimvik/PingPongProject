#include "gameLogic.h"

uint16_t goals = 0;
uint16_t lastGoal = clock();

uint16_t checkAndReturnGoals()
{
    if (readAdc() < 1069)
    {
        if (clock() - lastGoal >= MIN_GOAL_DELAY)
        {
            goals++;
        }
        lastGoal = clock();
    }

    return goals;
}
