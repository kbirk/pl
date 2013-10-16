#ifndef PL_COMMON_H
#define PL_COMMON_H

// MODEL MACROS  

#define PL_MODEL_BONE_COLOUR                     0.80, 0.80, 0.80
#define PL_MODEL_CARTILAGE_COLOUR                0.50, 0.20, 0.20       

// GRAFT MACROS

#define PL_GRAFT_DONOR_BONE_COLOUR               0.60, 0.90, 0.52 
#define PL_GRAFT_DONOR_CARTILAGE_COLOUR          0.30, 0.66, 0.22           
#define PL_GRAFT_DEFECT_BONE_COLOUR              0.90, 0.68, 0.27  
#define PL_GRAFT_DEFECT_CARTILAGE_COLOUR         0.80, 0.48, 0.07   

#define PL_GRAFT_DONOR_BONE_COLOUR_DULL          0.90, 1.00, 0.90 
#define PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL     0.70, 0.90, 0.60           
#define PL_GRAFT_DEFECT_BONE_COLOUR_DULL         1.00, 1.00, 0.70  
#define PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL    1.00, 0.88, 0.47 

#define PL_GRAFT_MARKER_COLOUR                   0.80, 0.20, 0.80   

#define PL_GRAFT_HANDLE_OFFSET                   6.0f

#define PL_GRAFT_EDIT_MODE_TRANSLATE             1
#define PL_GRAFT_EDIT_MODE_ROTATE                2 
#define PL_GRAFT_EDIT_MODE_LENGTH                3
#define PL_GRAFT_EDIT_MODE_MARKER                4

// BOUNDARY MACROS
 
#define PL_BOUNDARY_MESH_HEIGHT                  1.0f
#define PL_BOUNDARY_MESH_WIDTH_HALF              0.4f
#define PL_BOUNDARY_MESH_CURVE_HEIGHT            0.2f 
 
#define PL_BOUNDARY_SELECTED_COLOUR              0.20, 0.90, 0.70 
#define PL_BOUNDARY_POINT_SELECTED_COLOUR        0.20, 0.90, 0.70  
 
#define PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL    1.00, 0.50, 0.50
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL  0.85, 0.51, 0.85
#define PL_BOUNDARY_DONOR_COLOUR_DULL            0.70, 0.70, 1.00
#define PL_BOUNDARY_IGUIDE_COLOUR_DULL           0.50, 0.83, 0.70 
 
#define PL_BOUNDARY_DEFECT_CORNER_COLOUR         0.90, 0.20, 0.20
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR       0.55, 0.21, 0.55
#define PL_BOUNDARY_DONOR_COLOUR                 0.40, 0.40, 0.80
#define PL_BOUNDARY_IGUIDE_COLOUR                0.20, 0.53, 0.45

// COLOUR PICKING   

// TYPE
#define PL_PICKING_TYPE_NONE                     0
#define PL_PICKING_TYPE_BONE                     1
#define PL_PICKING_TYPE_CARTILAGE                2

// graft editor
#define PL_PICKING_TYPE_GRAFT_HANDLE_X           3
#define PL_PICKING_TYPE_GRAFT_HANDLE_Y           4
#define PL_PICKING_TYPE_GRAFT_HANDLE_Z           5

#define PL_PICKING_TYPE_GRAFT                    6

#define PL_PICKING_TYPE_DEFECT_SPLINE            7
#define PL_PICKING_TYPE_DEFECT_CORNERS           8
#define PL_PICKING_TYPE_DEFECT_BOUNDARY          9

#define PL_PICKING_TYPE_DEFECT_HANDLE_0          10
#define PL_PICKING_TYPE_DEFECT_HANDLE_1          11
#define PL_PICKING_TYPE_DEFECT_HANDLE_2          12
#define PL_PICKING_TYPE_DEFECT_HANDLE_3          13
#define PL_PICKING_TYPE_DEFECT_HANDLE_C          14

#define PL_PICKING_TYPE_DONOR_BOUNDARY           15
#define PL_PICKING_TYPE_IGUIDE_BOUNDARY          16

//INDEX 
#define PL_PICKING_INDEX_GRAFT_DONOR             1
#define PL_PICKING_INDEX_GRAFT_DEFECT            2 

// DRAW MACROS

#define PL_COLOUR_MESH_OPAQUE_COLOUR             0, 0, 0, 0
#define PL_COLOUR_MESH_TRANSPARENT_COLOUR        -1, -1, -1, 0

#define PL_HANDLE_RADIUS                         0.6f
#define PL_HANDLE_LENGTH                         7.0f
#define PL_ARROW_LENGTH                          3.0f
#define PL_CIRCLE_LENGTH                         1.0f
#define PL_HEAD_RADIUS                           2.0f
#define PL_SLICE_NUM                             30
#define PL_STACK_NUM                             1
 
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

#define PL_COLOURMAP_SIZE                        512
#define PL_PI                                    3.1415927f         // specific decimals optimize floating point accuracy. Going higher digits or using double will result in more error
#define PL_EPSILON                               0.00001f
#define PL_MIN_OF_2(a,b)                         (a<=b ? a : b)
#define PL_MAX_OF_2(a,b)                         (a>=b ? a : b)     
#define PL_MIN_OF_3(a,b,c)                       (a<=b ? (a<=c ? a : c) : (b<=c ? b : c))
#define PL_MAX_OF_3(a,b,c)                       (a>=b ? (a>=c ? a : c) : (b>=c ? b : c)) 
#define PL_ASPECT_RATIO                          ( 1280.0f / 720.0f ) 

#define PL_NORMAL_SMOOTHING_RADIUS               4.0f

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

#ifdef WIN32
    #include <windows.h>  
    #define PL_FILE_PREPATH     "../"
    inline float round( float val )
    {    
        return floor(val + 0.5f);
    }
#else
    #define PL_FILE_PREPATH     "./"        
#endif

#ifdef __APPLE_CC__
    #include <glew.h>    // include before gl.h
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/glew.h> // include before gl.h
    #include <GL/glut.h>
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif


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


// STD TYPEDEFS

template <typename T>
class plStack : public std::stack<T> {};

template <typename T>
class plSet : public std::set<T> {};


class plTimer
{
    public:
    
        static PLtime now()
        {
            typedef std::chrono::high_resolution_clock      plClock;
            typedef std::chrono::milliseconds               plMilliseconds;                
            return std::chrono::duration_cast<plMilliseconds>( plClock::now().time_since_epoch() ).count();
        }
        
    private:
    
        plTimer();
};


#endif
