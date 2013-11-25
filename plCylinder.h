#ifndef PL_CYLINDER_H
#define PL_CYLINDER_H

#include "plCommon.h"
#include "plRenderable.h"


class plCylinder : public plRenderable
{

    public:
    
        plCylinder( PLuint techniqueEnum, const plVector3 &origin, const plVector3 &direction, PLfloat radius, PLfloat length );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
    private:
    
        plVector3 _position;
        PLfloat   _radius; 
        plVector3 _direction; 
        PLfloat   _length;
        PLuint    _techniqueEnum;

        plVAO _generateVAO( float baseRadius, float topRadius, float height, int slices, int stacks ) const;    
    
};


#endif 
