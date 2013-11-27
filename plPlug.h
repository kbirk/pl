#ifndef PL_PLUG_H
#define PL_PLUG_H

#include "plCommon.h"
#include "plMeshSpecific.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

// TODO: FIXED DRAGGON TO INCLUDE A ROTATION OFFSET, ROTATION OFFSET CAPS AT n DEGREES AND IS MAINTAINED

class plPlug : public plMeshSpecific
{
    public:

        plPlug( PLuint type );
        plPlug( const plMesh& mesh, PLuint type, const plTransform &transform, const plVector3 surfaceNormal );

        const plTransform& transform() const { return _transform; }
        const plVector3& surfaceNormal() const { return _surfaceNormal; }

        void move( const plVector3& origin, const plVector3& y );
        //void rotate     ( const plVector3 &axis, PLfloat angleDegrees );
        
    private:
        
        PLuint      _type;
        plVector3   _surfaceNormal;
        plTransform _transform;    
     
};

#endif
