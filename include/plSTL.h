#pragma once

#include "plCommon.h"
#include "plMath.h"
#include "plString.h"
#include "plTriangle.h"
#include "plVector3.h"

namespace plSTL {
bool importFile(std::vector<plTriangle>& triangles, const plString& filename);
}
