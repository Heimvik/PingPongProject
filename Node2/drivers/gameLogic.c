#include "gameLogic.h"

uint16_t goals = 0;
uint64_t lastGoal = 0;

uint16_t checkAndReturnGoals()
{
    if (readAdc() < 1069)
    {
        if (time_now() - lastGoal >= MIN_GOAL_DELAY || lastGoal = 0)
        {
            goals++;
        }
        lastGoal = time_now();
    }

    return goals;
}
