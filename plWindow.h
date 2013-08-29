#ifndef __PL_WINDOW_H__
#define __PL_WINDOW_H__

#include "plCommon.h"
#include "plMatrix44.h"
#include "plRenderingPipeline.h"

namespace plWindow
{
    PLuint    width         ();
    PLuint    height        ();
    void      reshape       ( PLuint width, PLuint height );
    plVector3 mouseToWorld  ( PLint x, PLint y, PLint z );
    plVector3 worldToScreen ( PLfloat x, PLfloat y, PLfloat z );
    void      mouseToRay    ( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ); 				
}

#endif
