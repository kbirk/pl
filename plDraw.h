#ifndef __PL_DRAW_H__
#define __PL_DRAW_H__

#include "plCommon.h"
#include "plShapeMesh.h"
#include "plTrackedObject.h"

namespace plDraw
{

    void arrow    ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void arrow    ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void knob     ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void knob     ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void sphere   ( float radius);
    void sphere   ( const plVector3 &origin, float radius);
    void cylinder ( float radius, float height); 
    void cylinder ( const plVector3 &origin, const plVector3 &direction, float radius, float height);
    void disk     ( const plVector3 &origin, float scale, PLbool flip = false  );
    void disk     ( const plVector3 &origin, const plVector3 &direction, float scale, PLbool flip = false  );
    void disk     ( float scale, PLbool flip = false );

}

#endif
