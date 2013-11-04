#ifndef PLLASERLINE_H
#define PLLASERLINE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"

class plLaserLine
{

    public:
        plLaserLine();
        plLaserLine( const plVector3 &origin, const plVector3 &direction, PLfloat length = 10,
            const plVector4 &colour = plVector4( 1.0f, 0.0f, 0.0f, 1.0f ) );

        PLfloat     length;
        plVector3   origin;
        plVector3   direction;
        plVector4   colour;

};

#endif // PLLASERLINE_H
