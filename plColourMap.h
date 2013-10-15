#ifndef __PL_COLOUR_MAP_H__
#define __PL_COLOUR_MAP_H__

#include "plCommon.h"
#include "plVector3.h"

namespace plColourMap
{

    // clamps 0 <= k <= 1 and produce colour map value: 0 = blue, 1 = red
    plVector3 map( PLfloat k );

}

#endif
