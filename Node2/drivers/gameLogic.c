#include "gameLogic.h"

uint16_t goals = 0;
uint64_t lastGoal = time_now();

uint16_t checkAndReturnGoals()
{
    if (readAdc() < 1069)
    {
        if (time_now() - lastGoal >= MIN_GOAL_DELAY)
        {
            goals++;
        }
        lastGoal = time_now();
    }

    return goals;
}
