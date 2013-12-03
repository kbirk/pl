#ifndef PL_COMMON_H
#define PL_COMMON_H

// MODEL MACROS  

#define PL_MODEL_COLOUR                          0.80f, 0.80f, 0.80f
#define PL_MODEL_BONE_COLOUR                     0.80f, 0.80f, 0.80f
#define PL_MODEL_CARTILAGE_COLOUR                0.50f, 0.20f, 0.20f      

// GRAFT MACROS

#define PL_GRAFT_DONOR_BONE_COLOUR               0.60f, 0.90f, 0.52f 
#define PL_GRAFT_DONOR_CARTILAGE_COLOUR          0.30f, 0.66f, 0.22f           
#define PL_GRAFT_DEFECT_BONE_COLOUR              0.90f, 0.68f, 0.27f  
#define PL_GRAFT_DEFECT_CARTILAGE_COLOUR         0.80f, 0.48f, 0.07f   

#define PL_GRAFT_MARKER_COLOUR                   0.80f, 0.20f, 0.80f   

#define PL_GRAFT_EDIT_MODE_TRANSLATE             1
#define PL_GRAFT_EDIT_MODE_ROTATE                2 
#define PL_GRAFT_EDIT_MODE_LENGTH                3
#define PL_GRAFT_EDIT_MODE_RADIUS                4

#define PL_AXIS_GREY                             0.2f, 0.2f, 0.2f 
#define PL_X_AXIS_COLOUR                         1.0f, 0.2f, 0.2f
#define PL_Y_AXIS_COLOUR                         0.2f, 1.0f, 0.2f 
#define PL_Z_AXIS_COLOUR                         0.2f, 0.2f, 1.0f 

// BOUNDARY MACROS
 
#define PL_BOUNDARY_MESH_HEIGHT                  1.0f
#define PL_BOUNDARY_MESH_WIDTH_HALF              0.4f
#define PL_BOUNDARY_MESH_CURVE_HEIGHT            0.2f 
 
#define PL_BOUNDARY_SELECTED_COLOUR              0.20f, 0.90f, 0.70f
#define PL_BOUNDARY_POINT_SELECTED_COLOUR        0.20f, 0.90f, 0.70f 
 
#define PL_BOUNDARY_DEFECT_CORNER_COLOUR         0.90f, 0.20f, 0.20f
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR       0.55f, 0.21f, 0.55f
#define PL_BOUNDARY_DONOR_COLOUR                 0.40f, 0.40f, 0.80f
#define PL_BOUNDARY_IGUIDE_COLOUR                0.20f, 0.53f, 0.45f


// DRAW MACROS

#define PL_ASPECT_RATIO                          ( 1280.0f / 720.0f ) 
#define PL_NEAR_PLANE                            10.0f
#define PL_FAR_PLANE                             15000.0f
#define PL_FIELD_OF_VIEW                         7.0f

#define PL_LIGHT_POSITION                        10, 10, 15
#define PL_CLEAR_COLOUR                          0.137f, 0.137f, 0.137f, 1.0f
#define PL_EDITOR_MENU_HORIZONTAL_BUFFER         50
#define PL_EDITOR_MENU_VERTICAL_BUFFER           50
#define PL_EDITOR_MENU_HORIZONTAL_SPACING        40
#define PL_EDITOR_MENU_VERTICAL_SPACING          40   
#define PL_EDITOR_MENU_CIRCLE_RADIUS             14

#define PL_COLOUR_MESH_OPAQUE_COLOUR             0, 0, 0, 0
#define PL_COLOUR_MESH_TRANSPARENT_COLOUR        -1, -1, -1, 0

#define PL_HANDLE_SPHERE_RADIUS                  0.75f
#define PL_HANDLE_RADIUS                         0.3f
#define PL_HANDLE_LENGTH                         4.0f
#define PL_ARROW_LENGTH                          1.0f
#define PL_CIRCLE_LENGTH                         1.0f
#define PL_HEAD_RADIUS                           0.7f
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

#define PL_RAD_TO_DEG(a)                         ( a * ( 180.0f / PL_PI ) )
#define PL_DEG_TO_RAD(a)                         ( a * ( PL_PI / 180.0f ) )

#define PL_MIN_OF_2(a,b)                         (a<=b ? a : b)
#define PL_MAX_OF_2(a,b)                         (a>=b ? a : b)     
#define PL_MIN_OF_3(a,b,c)                       (a<=b ? (a<=c ? a : c) : (b<=c ? b : c))
#define PL_MAX_OF_3(a,b,c)                       (a>=b ? (a>=c ? a : c) : (b>=c ? b : c)) 

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
#include <iomanip>
#include <memory>
#include <map>


#ifdef WIN32

    #include <windows.h>  
    #define PL_FILE_PREPATH     "../"
    inline float round( float val )
    {    
        return floor( val + 0.5f );
    }
    #include <GL/glew.h> // include before gl.h
    #include <GL/gl.h>
    #include <GL/glut.h>

#else

    #define PL_FILE_PREPATH     "./"    
    #define GL_GLEXT_PROTOTYPES
    
    #ifdef __APPLE_CC__
        // APPLE

        #include <OpenGL/gl.h>
        #include <GLUT/glut.h>
        
    #else
    
        #include <GL/glut.h>
        #include <GL/glcorearb.h>
        #include <GL/glext.h>
        
    #endif

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


class plTimer
{
    public:
    
        static PLtime now()
        {
            typedef std::chrono::high_resolution_clock         plClock;
            typedef std::chrono::milliseconds                  plMilliseconds;                
            return std::chrono::duration_cast<plMilliseconds>( plClock::now().time_since_epoch() ).count();
        }
        
    private:
    
        plTimer();
};


#endif
