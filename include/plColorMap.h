#pragma once

#include "plCommon.h"
#include "plVector3.h"

#define PL_COLORMAP_SIZE 512

namespace plColorMap {

// clamps 0 <= k <= 1 and produce color map value: 0 = blue, 1 = red
plVector3 map(float32_t k);
}
