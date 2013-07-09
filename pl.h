#ifndef __PL_API_H__
#define __PL_API_H__

#include "plCommon.h"
  
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
 
#define PL_BOUNDARY_MESH_HEIGHT                  3.0f
#define PL_BOUNDARY_MESH_WIDTH_HALF              0.25f
#define PL_BOUNDARY_MESH_CURVE_HEIGHT            0.2f 
 
#define PL_BOUNDARY_SELECTED_COLOUR              0.20, 0.90, 0.70 
#define PL_BOUNDARY_POINT_SELECTED_COLOUR        0.20, 0.90, 0.70  
 
#define PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL    1.00, 0.50, 0.50
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL  0.85, 0.51, 0.85
#define PL_BOUNDARY_DONOR_COLOUR_DULL            0.70, 0.70, 1.00
#define PL_BOUNDARY_IGUIDE_COLOUR_DULL           1.00, 0.83, 1.00 
 
#define PL_BOUNDARY_DEFECT_CORNER_COLOUR         0.90, 0.20, 0.20
#define PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR       0.55, 0.21, 0.55
#define PL_BOUNDARY_DONOR_COLOUR                 0.40, 0.40, 0.80
#define PL_BOUNDARY_IGUIDE_COLOUR                0.80, 0.53, 0.95

// SPLINE MACROS

#define PL_DEFECT_SPLINE_COLOUR                  0.40, 0.20, 0.60

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

#define PL_PICKING_TYPE_EDIT_DEFECT_CORNERS      17
#define PL_PICKING_TYPE_EDIT_DEFECT_BOUNDARY     18
#define PL_PICKING_TYPE_EDIT_DONOR_BOUNDARY      19
#define PL_PICKING_TYPE_EDIT_IGUIDE_BOUNDARY     20

//INDEX 
#define PL_PICKING_INDEX_GRAFT_DONOR             1
#define PL_PICKING_INDEX_GRAFT_DEFECT            2 

// DRAW MACROS

#define PL_HANDLE_RADIUS                         0.6f
#define PL_HANDLE_LENGTH                         7.0f
#define PL_ARROW_LENGTH                          3.0f
#define PL_CIRCLE_LENGTH                         1.0f
#define PL_HEAD_RADIUS                           2.0f
#define PL_SLICE_NUM                             30
#define PL_STACK_NUM                             1
 
// OTHER MACROS

#define PL_COLOURMAP_SIZE                        512
#define PL_PI                                    3.1415927         
#define PL_MIN_OF_3(a,b,c)                       (a<=b ? (a<=c ? a : c) : (b<=c ? b : c))
#define PL_MAX_OF_3(a,b,c)                       (a>=b ? (a>=c ? a : c) : (b>=c ? b : c)) 
       
// TYPEDEFS

typedef unsigned char       PLbyte;      
typedef char                PLchar;      
typedef float               PLfloat;     
typedef double              PLdouble;    
typedef short unsigned int  PLushort;	
typedef short int           PLshort;		
typedef unsigned int        PLuint;     
typedef int                 PLint;       
typedef bool                PLbool;     
typedef std::string         plString;


//////////////////////////////////////////////////////////////////////////////
// GLOBAL OBJECTS
//////////////////////////////////////////////////////////////////////////////

template<class T>
class plSeq;
class plCamera;
class plVector3;
class plMatrix44;
class plPlan;
class plMatrixStack;
class plBoneAndCartilage;
class plProjection;
class plComputeShader;
class plMinimalShader;
class plPickingShader;
class plPickingTexture;
class plPickingInfo;
class plTransform;
class plMesh;
class plTriangle;
class plIntersection;
class plBoundary;
class plGraft;
class plGraftEditor;
class plBoundaryEditor;


extern plPlan           *_plPlan;
extern plCamera			*_plCamera;

extern plGraftEditor    *_plGraftEditor;
extern plBoundaryEditor *_plBoundaryEditor;

extern plMinimalShader  *_plMinimalShader;
extern plPickingShader  *_plPickingShader;
extern plComputeShader  *_plComputeShader;
extern plPickingInfo    *_plPickingState;
extern plPickingTexture *_plPickingTexture;

extern plProjection     *_plProjection;
extern plMatrixStack    *_plModelMatrixStack;
extern PLuint            _plColourmap[PL_COLOURMAP_SIZE][4];

//////////////////////////////////////////////////////////////////////////////
// INITIALIZE           pl.cpp
//////////////////////////////////////////////////////////////////////////////
extern void             plInit                  ();


//////////////////////////////////////////////////////////////////////////////
// Check for OpenGL Errors           pl.cpp
//////////////////////////////////////////////////////////////////////////////
extern void glErrorReport( char *where );


//////////////////////////////////////////////////////////////////////////////
// SET          
//////////////////////////////////////////////////////////////////////////////
extern void             plSet             ( plCamera &camera );
extern void             plSet             ( plPlan &plan );
extern void             plSet             ( plGraftEditor &graftEditor );
extern void             plSet             ( plBoundaryEditor &boundaryEditor );
extern void             plSet             ( plProjection &projection );


//////////////////////////////////////////////////////////////////////////////
// MODEL MATRIX			plMatrixStack.h
//////////////////////////////////////////////////////////////////////////////
extern void 			plSetMatrix 			( const plMatrix44 &matrix );
extern void				plMultMatrix    		( const plMatrix44 &matrix );
extern void		        plRotate		        ( PLfloat angle_degree, const plVector3 &axis );
extern void		        plRotate		        ( PLfloat angle_degree, PLfloat x, PLfloat y, PLfloat z );
extern void			    plTranslate	            ( const plVector3 &translation );
extern void			    plTranslate	            ( PLfloat x, PLfloat y, PLfloat z  );
extern void			    plScale		            ( const plVector3 &scale );
extern void			    plScale		            ( PLfloat x, PLfloat y, PLfloat z  );
extern void				plPushMatrix		    ();
extern void 			plPopMatrix			    ();


//////////////////////////////////////////////////////////////////////////////
// DRAW                 plDraw.h
//////////////////////////////////////////////////////////////////////////////
extern void             plDraw                   ( PLbool clear_buffer = true, PLbool leave_shader_bound = false);
extern void             plDrawSetViewingMatrix   ();
extern void             plDrawSetProjectionMatrix();
extern void             plDrawUnbindShader       ();
extern void             plDrawScope              ( const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible); 
extern void             plDrawProbe              ( const plVector3 &pos, const plVector3 &rotAxis, PLfloat rotAngle, PLbool visible);
 
extern void             plDrawArrow              ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawArrow              ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawCircleArrow        ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawCircleArrow        ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawSphere             ( float radius);
extern void             plDrawSphere             ( const plVector3 &origin, float radius);
extern void             plDrawCylinder           ( float radius, float height); 
extern void             plDrawDisk               ( const plVector3 &origin, float scale );
extern void             plDrawDisk               ( float scale );


//////////////////////////////////////////////////////////////////////////////
// WINDOW               plProjection.h
//////////////////////////////////////////////////////////////////////////////
extern void             plWindowReshape             ( PLuint width, PLuint height );
extern plVector3        plWindowGetMouseToWorldPos  ( PLint x, PLint y, PLint z = 0 );    
extern plVector3        plWindowGetWorldToScreenPos ( PLfloat x, PLfloat y, PLfloat z = 0 ); 


//////////////////////////////////////////////////////////////////////////////
// MATH                 plMath.h
//////////////////////////////////////////////////////////////////////////////  
extern plVector3        plProjectVectorOnPlane      ( const plVector3 &vector, const plVector3 &plane_normal);
extern plVector3        plClosestPointOnLineSegment ( const plVector3 &point, const plVector3 &a, const plVector3 &b);
extern PLfloat          plClamp                     ( PLfloat val, PLfloat min = -1.0f, PLfloat max = 1.0f);


//////////////////////////////////////////////////////////////////////////////
// STRING               plString.h
////////////////////////////////////////////////////////////////////////////// 
extern bool                  plStringCompareCaseInsensitive    ( const plString &str1, const plString &str2, PLuint num);
extern bool                  plStringCompareCaseInsensitive    ( const plString &str1, const plString &str2);
extern void                  plStringStripPreceedingWhitespace ( plString &s);
extern void                  plStringStripCharacter            ( plString &s, char c);
extern bool                  plStringOnlyWhitespace            ( const plString &s);
extern plString              plStringConcat                    ( const plString &s1, const plString &s2 );
extern plString              plStringConcat                    ( const plString &s1, const plString &s2,  const plString &s3);
template <class T> plString  plToString                        ( T t );
template <class T> T         plFromString                      ( const plString &str );


//////////////////////////////////////////////////////////////////////////////
// STL                  plTriangle.h
//////////////////////////////////////////////////////////////////////////////
extern void             plSTLImportFile             ( plSeq<plTriangle> &triangles, plString filename );
extern void             plSTLExportFileBinary       ( const plSeq<plTriangle> &triangles, plString filename );
extern void             plSTLExportFileASCII        ( const plSeq<plTriangle> &triangles, plString filename );


//////////////////////////////////////////////////////////////////////////////
// OTHER                
////////////////////////////////////////////////////////////////////////////// 
extern plVector3        plColourMap                 ( PLfloat i); 










#endif

