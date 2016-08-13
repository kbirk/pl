#pragma once

// COLOUR MACROS

#define PL_PURPLE_COLOUR                         0.57, 0.17, 0.93
#define PL_AXIS_GREY                             0.2f, 0.2f, 0.2f
#define PL_X_AXIS_COLOUR                         1.0f, 0.2f, 0.2f
#define PL_Y_AXIS_COLOUR                         0.2f, 1.0f, 0.2f
#define PL_Z_AXIS_COLOUR                         0.2f, 0.2f, 1.0f
#define PL_CLEAR_COLOUR                          0.137f, 0.137f, 0.137f, 1.0f

// some renderables use different colours per vertex. When the colour
// uniforms are set to these value, it tells the shader to use the attribute
// colour instead
#define PL_USE_ATTRIBUTE_COLOUR_OPAQUE           0, 0, 0, 0
#define PL_USE_ATTRIBUTE_COLOUR_TRANSPARENT      -1, -1, -1, 0

// global light position used throughout planner
#define PL_LIGHT_POSITION                        10, 10, 15

// IGUIDE BOOLEAN MESH OPERATIONS

#define PL_IGUIDE_BOOLEAN_MESH_DIFFERENCE        0
#define PL_IGUIDE_BOOLEAN_MESH_UNION             1
#define PL_IGUIDE_BOOLEAN_MESH_INTERSECTION      2

// LOGGING

#define PL_LOGGER_LEVEL_ERROR                    10
#define PL_LOGGER_LEVEL_WARNING                  20
#define PL_LOGGER_LEVEL_INFO                     30
#define PL_LOGGER_LEVEL_DEBUG                    40

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

#define PL_FILE_PREPATH                          "./resources/"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <vector>
#include <stack>
#include <limits>
#include <set>
#include <chrono>
#include <iomanip>
#include <memory>
#include <map>

#include <epoxy/gl.h>
#include <GL/glut.h>

// PRIMITIVE TYPEDEFS

typedef unsigned char           PLbyte;
typedef char                    PLchar;
typedef float                   PLfloat;
typedef double                  PLdouble;
typedef short unsigned int      PLushort;
typedef short int               PLshort;
typedef unsigned int            PLuint;
typedef int                     PLint;
typedef bool                    PLbool;
typedef long long unsigned int  PLtime;
