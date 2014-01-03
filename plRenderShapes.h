#ifndef PL_RENDER_SHAPES_H
#define PL_RENDER_SHAPES_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plVAO.h"


#define PL_HANDLE_SPHERE_RADIUS                  0.75f
#define PL_HANDLE_RADIUS                         0.3f
#define PL_HANDLE_LENGTH                         2.0f
#define PL_ARROW_LENGTH                          1.0f
#define PL_CIRCLE_LENGTH                         1.0f
#define PL_HEAD_RADIUS                           0.7f
#define PL_SLICE_NUM                             30
#define PL_STACK_NUM                             1


namespace plRenderShapes
{  
    plVAO sphereVAO  ( float radius, int slices, int stacks );
    plVAO cylinderVAO( float baseRadius, float topRadius, float height, int slices, int stacks );
    plVAO diskVAO    ( float innerRadius, float outerRadius, int slices, int loops, bool up = true );
    plVAO coneVAO    ( float baseRadius, float topRadius, float height, int slices, int stacks );  
    plVAO quadVAO    ();
    plVAO lineVAO    ( const plVector3& p0, const plVector3& p1 );
}


#endif

