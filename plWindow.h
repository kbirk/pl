#ifndef PL_WINDOW_H
#define PL_WINDOW_H

#include "plCommon.h"
#include "plMatrix44.h"
#include "plRenderingPipeline.h"

namespace plWindow
{

    PLuint    width            ();
    PLuint    height           ();    
    PLuint    viewportWidth    ();
    PLuint    viewportHeight   ();
    PLuint    viewportX        ();
    PLuint    viewportY        ();
    PLint     windowToViewportX( PLint x );
    PLint     windowToViewportY( PLint y );
    
    void      reshape          ( PLuint width, PLuint height );
    plVector3 mouseToWorld     ( PLint x, PLint y, PLint z );
    plVector3 worldToScreen    ( PLfloat x, PLfloat y, PLfloat z );
    plVector3 mouseToWorld     ( const plVector3& position );
    plVector3 worldToScreen    ( const plVector3& position );
    void      cameraToMouseRay ( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ); 				
}

#endif

