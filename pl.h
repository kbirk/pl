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
 
#define PL_GRAFT_NONE_SELECTED                   (_plState->graftSelectedID == -1)
#define PL_GRAFT_CURRENT_IS_SELECTED             (_plPickingState->id == _plState->graftSelectedID)
#define PL_GRAFT_SELECTED_IS_DEFECT              (_plState->graftSelectedIndex == PL_PICKING_INDEX_GRAFT_DEFECT)
#define PL_GRAFT_SELECTED_IS_DONOR               (_plState->graftSelectedIndex == PL_PICKING_INDEX_GRAFT_DONOR)
#define PL_GRAFT_IS_DEFECT                       (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DEFECT)
#define PL_GRAFT_IS_DONOR                        (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DONOR) 

#define PL_GRAFT_EDIT_SHOW_Y_HANDLE              ( (_plState->graftEditMode      == PL_GRAFT_EDIT_MODE_ROTATE &&      \
                                                    _plState->graftSelectedIndex == PL_PICKING_INDEX_GRAFT_DEFECT) || \
                                                   (_plState->graftEditMode      == PL_GRAFT_EDIT_MODE_TRANSLATE &&   \
                                                    _plState->graftSelectedIndex == PL_PICKING_INDEX_GRAFT_DEFECT) )
                                                                                               
#define PL_GRAFT_HANDLES_ENABLED                 (_plState->graftHandlesEnabled)

// BOUNDARY MACROS
 
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

#define PL_BOUNDARY_NONE_SELECTED                (_plState->boundarySelectedID == -1)
#define PL_BOUNDARY_CURRENT_IS_SELECTED          (_plPickingState->type == _plState->boundarySelectedType && \
                                                  _plPickingState->id   == _plState->boundarySelectedID)
                                                  
#define PL_BOUNDARY_POINT_CURRENT_IS_SELECTED    (_plPickingState->type  == _plState->boundarySelectedType && \
                                                  _plPickingState->id    == _plState->boundarySelectedID &&   \
                                                  _plPickingState->index == _plState->boundarySelectedPointID)

// SPLINE MACROS

#define PL_DEFECT_SPLINE_COLOUR                  0.40, 0.20, 0.60

#define PL_DEFECT_SPLINE_CURRENT_IS_SELECTED     (_plState->splineSelectedID == _plPickingState->id )

// COLOUR PICKING   

// TYPE
#define PL_PICKING_TYPE_NONE                     0
#define PL_PICKING_TYPE_BONE                     1
#define PL_PICKING_TYPE_CARTILAGE                2
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

template<class T> class plSeq;
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
class plState;
class plTransform;
class plMesh;
class plTriangle;
class plIntersection;

extern plMesh                         *_plSphere;
extern plMesh                         *_plCylinder;
extern plMesh                         *_plDisk;
extern plState                        *_plState;
extern plMinimalShader                *_plMinimalShader;
extern plPickingShader                *_plPickingShader;
extern plComputeShader                *_plTestComputeShader;
extern plPickingInfo                  *_plPickingState;
extern plPickingTexture               *_plPickingTexture;
extern plCamera						  *_plCamera;
extern plProjection			     	  *_plProjection;
extern plMatrixStack			      *_plModelMatrixStack;
extern PLuint                         _plColourmap[PL_COLOURMAP_SIZE][4];

extern plPlan                         *_plPlan;
extern plSeq<plBoneAndCartilage*> 	  _plBoneAndCartilageModels;




//////////////////////////////////////////////////////////////////////////////
// INITIALIZE           pl.cpp
//////////////////////////////////////////////////////////////////////////////
extern void             plInit                  ();


//////////////////////////////////////////////////////////////////////////////
// CAMERA MATRIX: 		plCamera.h
//////////////////////////////////////////////////////////////////////////////
extern plVector3		plCameraGetDirection	();
extern plVector3        plCameraGetPosition     ();
extern void			    plCameraSetPosition		( const plVector3 &position);
extern void			    plCameraSetFocus		( const plVector3 &focus);
extern void			    plCameraSetUp   		( const plVector3 &up);
extern void				plCameraTranslate		( PLint x, PLint y );
extern void				plCameraZoom		    ( PLfloat z );
extern void				plCameraArcballRotate	( PLint x0, PLint x1, PLint y0, PLint y1 );
extern void				plCameraImportView		( plString filename );
extern void				plCameraExportView		( plString filename );
extern void				plCameraResetToModel	( PLuint model_id );


//////////////////////////////////////////////////////////////////////////////
// PROJECTION MATRIX:	plProjection.h
//////////////////////////////////////////////////////////////////////////////
extern plMatrix44		plProjectionGetMatrix	();
extern void             plProjectionSetAspect   (PLfloat aspect_ratio);
extern void 			plProjectionSetFOV 	    (PLfloat fov);
extern void 			plProjectionSetNear     (PLfloat near_plane);
extern void 			plProjectionSetFar      (PLfloat far_plane);
extern void 			plProjectionSetPerspective  ( PLfloat fov, PLfloat aspect, PLfloat near_plane, PLfloat far_plane );


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
extern void             plDraw                   ();
extern void             plDrawSetViewingMatrix   ();
extern void             plDrawSetProjectionMatrix();

extern void             plDrawArrow              ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawArrow              ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawCircleArrow        ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawCircleArrow        ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
extern void             plDrawSphere             ( float radius);
extern void             plDrawSphere             ( const plVector3 &origin, float radius);
extern void             plDrawCylinder           ( float radius, float height); 
extern void             plDrawDisk               ( float scale );


//////////////////////////////////////////////////////////////////////////////
// MODELS				plModel.h
//////////////////////////////////////////////////////////////////////////////
extern void             plModelAdd              ( plString bone_file, plString cartilage_file );
extern void             plModelRemove           ( PLuint model_id );
extern PLuint			plModelCount			();
extern void             plModelSelect           ( PLuint model_id );
extern PLint            plModelGetSelectedID    ();
extern void             plModelBoneToggleVisibility       ( PLuint model_id );
extern void             plModelCartilageToggleVisibility  ( PLuint model_id );

//extern PLbool           plModelBoneIntersect      ( plVector3 &point, plVector3 &normal, PLuint model_id, const plVector3 &ray_origin, const plVector3 &direction, PLbool ignore_behind_ray = false, PLbool backface_cull = false);
//extern PLbool           plModelCartilageIntersect ( plVector3 &point, plVector3 &normal, PLuint model_id, const plVector3 &ray_origin, const plVector3 &direction, PLbool ignore_behind_ray = false, PLbool backface_cull = false);
extern plIntersection   plModelBoneIntersect     (PLuint model_id, const plVector3 &ray_origin, const plVector3 &direction, PLbool ignore_behind_ray=false, PLbool backface_cull=false );
extern plIntersection   plModelCartilageIntersect(PLuint model_id, const plVector3 &ray_origin, const plVector3 &direction, PLbool ignore_behind_ray=false, PLbool backface_cull=false );
                        
extern plVector3        plModelBoneGetAvgNormal      ( PLuint model_id, PLfloat radius, const plVector3 &origin, const plVector3 &up );
extern plVector3        plModelCartilageGetAvgNormal ( PLuint model_id, PLfloat radius, const plVector3 &origin, const plVector3 &up );

//extern void             plModelDraw             ( PLuint model_id );
//extern void             plModelDrawAll          ();


//////////////////////////////////////////////////////////////////////////////
// PLAN                 plPlan.h
//////////////////////////////////////////////////////////////////////////////
extern void             plPlanImport            ( plString plan_file );
extern void             plPlanExport            ( plString plan_file );
extern void             plPlanCreateNew         ();
extern void             plPlanToggleVisibility  ();


//////////////////////////////////////////////////////////////////////////////
// GRAFT
//////////////////////////////////////////////////////////////////////////////
extern PLuint			plGraftCount			();
extern void             plGraftSelectRecipient  ( PLuint graft_id );
extern void             plGraftSelectHarvest    ( PLuint graft_id );
extern PLbool           plGraftIsSelected       ();
extern PLint            plGraftGetSelectedID    ();
extern void             plGraftEnableHandles    ();
extern void             plGraftDisableHandles   ();

extern void             plGraftSetRotateMode    ();
extern void             plGraftSetTranslateMode ();
extern void             plGraftSetLengthMode    ();
extern void             plGraftSetMarkerMode    ();

extern void             plGraftTranslate        ( PLuint graft_id, const plVector3 &translation );
extern void             plGraftTranslate        ( const plVector3 &translation );
extern void             plGraftSurfaceTranslate ( PLuint graft_id, PLuint graft_index, const plVector3 &translation );
extern void             plGraftSurfaceTranslate ( const plVector3 &translation );
extern void             plGraftRotate           ( PLuint graft_id, const plVector3 &axis, PLfloat angle_degrees );
extern void             plGraftRotate           ( const plVector3 &axis, PLfloat angle_degrees );
extern void             plGraftSpinMarker       ( PLuint graft_id, PLfloat angle_degrees );
extern void             plGraftSpinMarker       ( PLfloat angle_degrees );

extern plTransform      plGraftGetHarvestTransform   ( PLuint graft_id );
extern plTransform      plGraftGetRecipientTransform ( PLuint graft_id );

extern plVector3        plGraftGetHarvestXAxis  ( PLuint graft_id );
extern plVector3        plGraftGetHarvestYAxis  ( PLuint graft_id );
extern plVector3        plGraftGetHarvestZAxis  ( PLuint graft_id );
extern plVector3        plGraftGetHarvestOrigin ( PLuint graft_id );

extern plVector3        plGraftGetRecipientXAxis  ( PLuint graft_id );
extern plVector3        plGraftGetRecipientYAxis  ( PLuint graft_id );
extern plVector3        plGraftGetRecipientZAxis  ( PLuint graft_id );
extern plVector3        plGraftGetRecipientOrigin ( PLuint graft_id );

extern void             plGraftSetDragOrigin      ( PLint x, PLint y ); 
extern void             plGraftDragEdit           ( PLint x, PLint y ); 

extern void             plGraftToggleVisibility   ( PLuint graft_id );
extern void             plGraftToggleVisibilityAll();

//////////////////////////////////////////////////////////////////////////////
// DONOR REGION
//////////////////////////////////////////////////////////////////////////////
extern void             plDonorRegionAdd        ();
extern void             plDonorRegionRemove     ( PLuint region_id );
extern PLuint			plDonorRegionCount		();
extern void             plDonorRegionToggleVisibility( PLuint region_id );
extern void             plDonorRegionToggleVisibilityAll();

//////////////////////////////////////////////////////////////////////////////
// DEFECT SPLINE
//////////////////////////////////////////////////////////////////////////////
extern void             plDefectSplineAdd       ();
extern void             plDefectSplineRemove    ();
extern PLuint			plDefectSplineCount		();
extern void             plDefectSplineToggleVisibility           ( PLuint spline_id );
extern void             plDefectSplineToggleVisibilityAll        ();
extern void             plDefectSplineCornersToggleVisibility    ( PLuint spline_id );
extern void             plDefectSplineCornersToggleVisibilityAll ();
extern void             plDefectSplineBoundaryToggleVisibility   ( PLuint spline_id );
extern void             plDefectSplineBoundaryToggleVisibilityAll();
extern PLbool           plDefectSplineIsSelected();
extern void             plDefectSplineSetDragOrigin( PLint x, PLint y );


//////////////////////////////////////////////////////////////////////////////
// BOUNDARY POINT
//////////////////////////////////////////////////////////////////////////////
extern PLint            plBoundaryPointAdd      ( const plVector3 &point, const plVector3 &normal );
extern PLint            plBoundaryPointAdd      ( PLuint x, PLuint y );
extern void             plBoundaryPointSelect   ( PLuint index );
extern PLint            plBoundaryGetSelectedType ();
extern void             plBoundaryPointMove     ( PLuint x, PLuint y );
extern void             plBoundaryPointMove     ( const plVector3 &point, const plVector3 &normal );
extern void             plBoundaryPointRemove   ( PLuint point_index );
extern void             plBoundaryPointRemove   ();


//////////////////////////////////////////////////////////////////////////////
// PICKING		        plPickingTexture.h
//////////////////////////////////////////////////////////////////////////////
extern PLint            plPickingGetType        ( PLuint x, PLuint y );
extern PLint            plPickingGetID          ( PLuint x, PLuint y );
extern PLint            plPickingGetIndex       ( PLuint x, PLuint y );
extern GLfloat          plPickingGetDepth       ( PLuint x, PLuint y );
extern PLint            plPickingSelect         ( PLuint x, PLuint y ); 


//////////////////////////////////////////////////////////////////////////////
// WINDOW               plProjection.h
//////////////////////////////////////////////////////////////////////////////
extern void             plWindowReshape             ( PLuint width, PLuint height );
extern plVector3        plWindowGetMouseToWorldPos  ( PLint x, PLint y, PLint z = 0 );    
extern plVector3        plWindowGetWorldToScreenPos ( PLfloat x, PLfloat y, PLfloat z = 0 ); 


//////////////////////////////////////////////////////////////////////////////
// ERROR
//////////////////////////////////////////////////////////////////////////////                
extern PLbool           plErrorCheckGraftBounds     ( PLuint index, plString function_name );     
extern PLbool           plErrorCheckModelBounds     ( PLuint index, plString function_name ); 
extern PLbool           plErrorCheckDonorRegionBounds ( PLuint index, plString function_name );
extern PLbool           plErrorCheckDefectSplineBounds( PLuint index, plString function_name );

extern PLbool           plErrorIsGraftSelected      ( plString function_name );
extern PLbool           plErrorIsBoundarySelected   ( plString function_name );
extern PLbool           plErrorIsBoundaryPointSelected( PLuint index, plString function_name );
extern PLbool           plErrorIsDefectSplineSelected ( PLuint index, plString function_name );


//////////////////////////////////////////////////////////////////////////////
// MATH
//////////////////////////////////////////////////////////////////////////////  
extern plVector3        plProjectVectorOnPlane      ( const plVector3 &vector, const plVector3 &plane_normal);
extern plVector3        plClosestPointOnLineSegment ( const plVector3 &point, const plVector3 &a, const plVector3 &b);


//////////////////////////////////////////////////////////////////////////////
// STRING
////////////////////////////////////////////////////////////////////////////// 
extern bool             plStringCompareCaseInsensitive    ( const plString &str1, const plString &str2, PLuint num);
extern bool             plStringCompareCaseInsensitive    ( const plString &str1, const plString &str2);
extern void             plStringStripPreceedingWhitespace ( plString &s);
extern void             plStringStripCharacter            ( plString &s, char c);
extern bool             plStringOnlyWhitespace            ( const plString &s);


//////////////////////////////////////////////////////////////////////////////
// STL
//////////////////////////////////////////////////////////////////////////////
extern void             plSTLImportFile             ( plSeq<plTriangle> &triangles, plString filename );
extern void             plSTLExportFileBinary       ( const plSeq<plTriangle> &triangles, plString filename );
extern void             plSTLExportFileASCII        ( const plSeq<plTriangle> &triangles, plString filename );


//////////////////////////////////////////////////////////////////////////////
// OTHER
////////////////////////////////////////////////////////////////////////////// 
extern plVector3        plColourMap                 ( PLfloat i); //, PLfloat max);










#endif

