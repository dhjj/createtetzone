#include "StatusLineUpdater.h"
#include "TECADDON.h"
#include "Lock.h"

void StatusLineUpdater::begin()
{
    Lock lock;
    TecUtilStatusStartPercentDone("Creating tetrahedral zone...",
                                  true,  // show stop
                                  true); // show progress
}

void StatusLineUpdater::end()
{
    Lock lock;
    TecUtilStatusFinishPercentDone();
}

bool StatusLineUpdater::update(int percentDone)
{
    Lock lock;
    TecUtilStatusCheckPercentDone(percentDone);

    bool keepGoing = TecUtilInterruptCheck() == FALSE;
    return keepGoing;
}
