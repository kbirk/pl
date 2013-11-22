#ifndef PL_CONE_H
#define PL_CONE_H

#include "plCommon.h"
#include "plRenderable.h"


class plCone : public plRenderable
{

    public:
    
        plCone( PLuint techniqueEnum, const plVector3 &origin, const plVector3 &direction, PLfloat bottomRadius, PLfloat topRadius, PLfloat length );

        void extractRenderComponents( plRenderMap& renderMap ) const;

    private:
    
        plVector3 _position;
        PLfloat   _bottomRadius; 
        PLfloat   _topRadius; 
        plVector3 _direction; 
        PLfloat   _length;
        PLuint    _techniqueEnum;

        plVAO _generateVAO( float baseRadius, float topRadius, float height, int slices, int stacks ) const;     
    
};


#endif 
