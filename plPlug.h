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

        plPlug();
        plPlug( PLuint _modelID, const plBoneAndCartilage &_model, const plTransform &t );

        const plTransform& transform()  const { return _transform; }

        void move       ( const plVector3 &origin, const plVector3 &y );
        //void translate  ( const plVector3 &translation );
        //void translateX ( PLfloat distance, const plVector3 &planeNormal );
        //void translateZ ( PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( const plVector3 &axis, PLfloat angleDegrees );
        
    private:
                       
        plTransform _transform;         // used to cache transform, also allows references / pointers to this      
        //plTransform _surfaceTransform;         
        //plTransform _rotationTransform;
    
        //void   _updateTransform()  { _transform = _surfaceTransform * _rotationTransform; }
        PLbool _surfaceIntersection( plVector3 &point, plVector3 &normal, const plVector3 &translation ) const;       
};

#endif
