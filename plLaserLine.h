#ifndef PL_LASERLINE_H
#define PL_LASERLINE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"

class plLaserLine : public plRenderable
{

    public:
    
        plLaserLine( const plVector3 &position, const plVector3 &direction, PLfloat length = 10 );

        void extractRenderComponents( plRenderMap& renderMap ) const;

    private:  
    
        PLuint      _techniqueEnum; 
        PLfloat     _length;
        plVector3   _position;
        plVector3   _direction;

};

#endif
