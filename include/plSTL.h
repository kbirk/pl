#pragma once

#include "plCommon.h"
#include "plString.h"
#include "plVector3.h"
#include "plMath.h"
#include "plTriangle.h"

namespace plSTL
{
    bool importFile(std::vector<plTriangle>& triangles, const plString& filename);
}
