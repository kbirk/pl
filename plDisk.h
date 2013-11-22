#ifndef PL_DISK_H
#define PL_DISK_H

#include "plCommon.h"
#include "plRenderable.h"


class plDisk : public plRenderable
{

    public:
    
        plDisk( PLuint techniqueEnum, const plVector3 &position, PLfloat radius = 1.0f, PLbool flip = false );

        void extractRenderComponents( plRenderMap& renderMap ) const;

    private:
    
        plVector3 _position;
        PLfloat   _radius; 
        PLbool    _flip;
        PLuint    _techniqueEnum;

        plVAO _generateVAO( float innerRadius, float outerRadius, int slices, int loops, bool up = true ) const;
    
};


#endif 
