#pragma once

// COLOR MACROS

#define PL_PURPLE_COLOR 0.57, 0.17, 0.93
#define PL_AXIS_GREY 0.2f, 0.2f, 0.2f
#define PL_X_AXIS_COLOR 1.0f, 0.2f, 0.2f
#define PL_Y_AXIS_COLOR 0.2f, 1.0f, 0.2f
#define PL_Z_AXIS_COLOR 0.2f, 0.2f, 1.0f
#define PL_CLEAR_COLOR 0.137f, 0.137f, 0.137f, 1.0f

// some renderables use different colors per vertex. When the color uniform is
// set to these values, it tells the shader to use the attribute colors instead
#define PL_USE_ATTRIBUTE_COLOR 0, 0, 0, 0

// global light position used throughout planner
#define PL_LIGHT_POSITION 10, 10, 15

// OTHER MACROS

#define PL_PI 3.1415927f
#define PL_EPSILON 0.00001f

#define PL_RAD_TO_DEG(a) (a * (180.0f / PL_PI))
#define PL_DEG_TO_RAD(a) (a * (PL_PI / 180.0f))

#define PL_MIN_OF_2(a, b) (a <= b ? a : b)
#define PL_MAX_OF_2(a, b) (a >= b ? a : b)
#define PL_MIN_OF_3(a, b, c) (a <= b ? (a <= c ? a : c) : (b <= c ? b : c))
#define PL_MAX_OF_3(a, b, c) (a >= b ? (a >= c ? a : c) : (b >= c ? b : c))

#define PL_NORMAL_SMOOTHING_RADIUS 4.0f

// logger
#include "plLog.h"

// stl
#include <algorithm>
#include <cctype>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

// PRIMITIVE TYPEDEFS

// Precision explicit typedefs
typedef float float32_t;
typedef double float64_t;

// Fixed width integer types
#include <cstdint>
