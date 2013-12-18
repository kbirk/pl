#ifndef PL_PLUG_H
#define PL_PLUG_H

#include "plCommon.h"
#include "plMeshSpecific.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

// TODO: FIXED DRAGGING TO INCLUDE A ROTATION OFFSET, ROTATION OFFSET CAPS AT n DEGREES AND IS MAINTAINED

#define PL_MAX_GRAFT_ROTATION                    25.0f

class plPlug : public plMeshSpecific
{
    public:

        plPlug( PLuint type );
        plPlug( const plMesh& mesh, PLuint type, const plTransform& surfaceTransform, const plTransform& rotationalOffset );

        const plTransform& surfaceTransform() const; 
        const plTransform& offsetTransform() const;        
        plTransform        finalTransform() const; 

        void move  ( const plVector3& origin, const plVector3& y );
        void rotate( const plVector3& y );
        void rotate( PLfloat angleDegrees );
        
    private:
        
        PLuint      _type;        
        plTransform _surfaceTransform;
        plTransform _rotationalOffset;       

};


#endif
