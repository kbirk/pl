#pragma once

#include "plCommon.h"
#include "plTimer.h"

namespace plProgress
{
    void startProgress();

    void endProgress();

    void printProgress(float32_t percent);
}
