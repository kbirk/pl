#ifndef PLDEBUGSPHERE_H
#define PLDEBUGSPHERE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"

class plDebugSphere
{

    public:

        plDebugSphere( const plVector3 &origin,
                       PLfloat radius = 1.0f,
                       const plVector4 &colour = plVector4( 0.5f, 0.5f, 0.5f, 1.0f ) );

        PLfloat     radius;
        plVector3   origin;
        plVector4   colour;

};

#endif // PLDEBUGSPHERE_H
