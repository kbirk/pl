#ifndef PL_DEBUG_SPHERE_H
#define PL_DEBUG_SPHERE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plDraw.h"

class plDebugSphere
{

    public:

        plDebugSphere( const plVector3 &origin, PLfloat radius = 1.0f, const plVector4 &colour = plVector4( 0.5f, 0.5f, 0.5f, 1.0f ) );
    
        void draw() const;
    
    private:
    
        PLfloat     _radius;
        plVector3   _origin;
        plVector4   _colour;

};

#endif
