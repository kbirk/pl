#pragma once

#include "plCommon.h"
#include "plVector3.h"

#define PL_COLOURMAP_SIZE                        512

namespace plColourMap
{

    // clamps 0 <= k <= 1 and produce colour map value: 0 = blue, 1 = red
    plVector3 map(PLfloat k);

}
