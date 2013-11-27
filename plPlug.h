#ifndef PL_PLUG_H
#define PL_PLUG_H

#include "plCommon.h"
#include "plModelSpecific.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

// TODO: FIXED DRAGGON TO INCLUDE A ROTATION OFFSET, ROTATION OFFSET CAPS AT n DEGREES AND IS MAINTAINED

class plPlug : public plModelSpecific
{
    public:

        plPlug( PLuint type );
        plPlug( PLuint modelID, const plBoneAndCartilage &model, PLuint type, const plTransform &transform, const plVector3 surfaceNormal );

        const plTransform& transform() const { return _transform; }
        const plVector3& surfaceNormal() const { return _surfaceNormal; }

        void move       ( const plVector3& origin, const plVector3& y, const plVector3& surfaceNormal );
        //void rotate     ( const plVector3 &axis, PLfloat angleDegrees );
        
    private:
        
        PLuint      _type;
        plVector3   _surfaceNormal;
        plTransform _transform;    
     
};

#endif
