#ifndef PL_SPHERE_H
#define PL_SPHERE_H

#include "plCommon.h"
#include "plRenderable.h"


class plSphere : public plRenderable
{

    public:
    
        plSphere( PLuint techniqueEnum, const plVector3 &position, PLfloat radius = 1.0f );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        void position( const plVector3 position );
        void radius  ( PLfloat radius  );

    private:
    
        plVector3 _position;
        PLfloat   _radius; 
        PLuint    _techniqueEnum;

        plVAO _generateVAO( float radius, int slices, int stacks ) const;     
    
};


#endif 
