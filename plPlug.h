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

        plTransform transform; 

        plPlug();
        plPlug( PLuint _modelID, const plBoneAndCartilage &_model, const plTransform &t );

        void translate  ( const plVector3 &translation );
        void translateX ( PLfloat distance, const plVector3 &planeNormal );
        void translateZ ( PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( const plVector3 &axis, PLfloat angleDegrees );
        
    private:
    
        PLbool _surfaceIntersection( plVector3 &point, plVector3 &normal, const plVector3 &translation ) const;       
};

#endif
