#include "GrumpyConfig.h"
#include "FpsTracker.h"

using namespace puddi;

namespace grumpy
{
    // PUBLIC

    void GrumpyConfig::SetGameSpeed(float s)
    {
        gameSpeed = s;
    }
    float GrumpyConfig::GetGameSpeed()
    {
        return gameSpeed;
    }

    float GrumpyConfig::GetGameSpeedFactor()
    {
        return gameSpeed * FpsTracker::GetFrameTimeMs();
    }

    // PRIVATE

    float GrumpyConfig::gameSpeed = 1.0f;
}
