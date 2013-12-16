#ifndef PL_RENDER_SHAPES_H
#define PL_RENDER_SHAPES_H

#include "plCommon.h"
#include "plVector3.h"
#include "plRenderable.h"
#include "plVAO.h"

namespace plRenderShapes
{  
    plVAO sphereVAO  ( float radius, int slices, int stacks );
    plVAO cylinderVAO( float baseRadius, float topRadius, float height, int slices, int stacks );
    plVAO diskVAO    ( float innerRadius, float outerRadius, int slices, int loops, bool up = true );
    plVAO coneVAO    ( float baseRadius, float topRadius, float height, int slices, int stacks );  
    plVAO quadVAO    ();
}


#endif

