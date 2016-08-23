#pragma once

// COLOR MACROS

#define PL_PURPLE_COLOR                         0.57, 0.17, 0.93
#define PL_AXIS_GREY                            0.2f, 0.2f, 0.2f
#define PL_X_AXIS_COLOR                         1.0f, 0.2f, 0.2f
#define PL_Y_AXIS_COLOR                         0.2f, 1.0f, 0.2f
#define PL_Z_AXIS_COLOR                         0.2f, 0.2f, 1.0f
#define PL_CLEAR_COLOR                          0.137f, 0.137f, 0.137f, 1.0f

// some renderables use different colors per vertex. When the color
// uniforms are set to these value, it tells the shader to use the attribute
// color instead
#define PL_USE_ATTRIBUTE_COLOR_OPAQUE           0, 0, 0, 0

// global light position used throughout planner
#define PL_LIGHT_POSITION                        10, 10, 15

// OTHER MACROS

#define PL_PI                                    3.1415927f         // specific decimals optimize floating point accuracy. Going higher digits will result in more error
#define PL_EPSILON                               0.00001f

#define PL_RAD_TO_DEG(a)                         (a * (180.0f / PL_PI))
#define PL_DEG_TO_RAD(a)                         (a * (PL_PI / 180.0f))

#define PL_MIN_OF_2(a,b)                         (a<=b ? a : b)
#define PL_MAX_OF_2(a,b)                         (a>=b ? a : b)
#define PL_MIN_OF_3(a,b,c)                       (a<=b ? (a<=c ? a : c) : (b<=c ? b : c))
#define PL_MAX_OF_3(a,b,c)                       (a>=b ? (a>=c ? a : c) : (b>=c ? b : c))

#define PL_NORMAL_SMOOTHING_RADIUS               4.0f

#include <csignal>
#include <cctype>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <limits>
#include <set>
#include <list>
#include <chrono>
#include <iomanip>
#include <memory>
#include <map>

// PRIMITIVE TYPEDEFS

// Precision explicit typedefs
typedef float float32_t;
typedef double float64_t;

// Fixed width integer types
#include <cstdint>
